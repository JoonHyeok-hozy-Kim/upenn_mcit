#!/bin/bash

# Slurm setup
#SBATCH -p gu-compute
#SBATCH -A gu-account
#SBATCH --qos=gu-med
#SBATCH --gres=gpu:1
#SBATCH --mem=64G
#SBATCH --time=4:00:00
#SBATCH --output=/home/hozy/torch_setup/logs/pytorch_build_%j.log

echo "==================================="
echo "Job name : Build PyTorch"
date
echo "Job running on node: $(hostname)"
echo "==================================="

CONDA_ENV_NAME="torch_blackwell_default"
echo "Attempting to activate Conda env: $CONDA_ENV_NAME"
source ~/miniconda3/bin/activate $CONDA_ENV_NAME

echo "[DEBUG] Conda env check:"
echo "CONDA_DEFAULT_ENV: $CONDA_DEFAULT_ENV"

echo "[DEBUG] Python path check:"
type python
echo "==================================="

echo "Enforcing GLIBC/OpenMP compatibility..."
export LD_PRELOAD=$CONDA_PREFIX/lib/libgomp.so:$CONDA_PREFIX/lib/libstdc++.so:$LD_PRELOAD
export LDFLAGS="-L$CONDA_PREFIX/lib $LDFLAGS"
export CXXFLAGS="-fPIC $CXXFLAGS" 

export CUDACXX=$CONDA_PREFIX/bin/nvcc
export CMAKE_PREFIX_PATH=$CONDA_PREFIX

echo "------------------------------------"

echo "Add LD_LIBRARY_PATH"
export SYSTEM_LIB_PATH="/usr/lib/x86_64-linux-gnu"
export CONDA_LIB_PATH="$CONDA_PREFIX/lib"
export LD_LIBRARY_PATH="$SYSTEM_LIB_PATH:$CONDA_LIB_PATH:$LD_LIBRARY_PATH"

echo "[DEBUG] LD_LIBRARY_PATH set to: $LD_LIBRARY_PATH"
echo "==================================="

PYTORCH_DIR="/scratch/tmp/hozy/torch_setup/pytorch"
echo "Attempting to change directory to: $PYTORCH_DIR"
cd $PYTORCH_DIR || { echo "ERROR: Could not find $PYTORCH_DIR. Exiting."; exit 1; }

echo "[DEBUG] Current directory check:"
pwd

echo "[DEBUG] Directory contents check :"
ls -ld setup.py

echo "==================================="

echo "Clean PyTorch repo."
git config --global --add safe.directory /scratch/tmp/hozy/torch_setup/pytorch
git clean -xdf

echo "Start building"
pip install . -v

echo "Fin."
date