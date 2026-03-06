#!/usr/bin/bash

# this file is meant to be sourced on your workstation to enable helper commands/env vars for interacting with gcloud hosts

if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "This script should be sourced, not executed:"
    echo "  source ${BASH_SOURCE[0]}"
    exit 1
fi
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ ! -f "$SCRIPT_DIR/env.sh" ]]; then
    echo "Missing env.sh — copy env.sh.example to env.sh and fill in your values."
    return 1
fi
source "$SCRIPT_DIR/env.sh"

gtpu() {
    local worker_flag=""
    
    if [[ "$1" == "-a" ]]; then
        worker_flag="--worker=all"
        shift
    elif [[ "$1" =~ ^-([0-9]+)$ ]]; then
        worker_flag="--worker=${BASH_REMATCH[1]}"
        shift
    fi
    
    gcloud compute tpus tpu-vm "$@" --zone="$ZONE" --project="$PROJECT" $worker_flag
}

# scps a file from the workstation to all remotes
gpush() {
    local scp_flags=()
    if [[ "$1" == "-r" ]]; then
        scp_flags+=("--recurse")
        shift
    fi

    gtpu -a scp "${scp_flags[@]}" "$@" "$TPU_NAME":~/
}

gpython() {
    local skip_scp=false
    if [[ "$1" == "-r" ]]; then
        skip_scp=true
        shift
    fi

    local script="$1"
    shift
    local basename="${script##*/}"
    if [[ "$skip_scp" == false ]]; then
        gpush "$script"
    fi

    # The worker_setup.sh script sets up a venv for a more recent version of python in jax-env; we use this when running our scripts
    gtpu -a ssh "$TPU_NAME" --command="source ~/jax-env/bin/activate && python3 ~/$basename $*"
}

# Assumes there are 4 workers
gettraces() {
    local remote_path="${1:-/tmp/jax-trace}"
    local local_path="${2:-/tmp/jax-trace-worker}"
    mkdir -p "$local_path"
    for i in 0 1 2 3; do
        gtpu -$i scp --recurse "$TPU_NAME":"$remote_path" "$local_path"/
    done
    gtpu -a ssh "$TPU_NAME" --command="rm -rf '$remote_path'"
    echo "Copied traces to '$local_path$'"
}
