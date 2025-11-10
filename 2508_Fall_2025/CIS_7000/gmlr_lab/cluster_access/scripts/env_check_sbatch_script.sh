#!/bin/bash

# Slurm setup
#SBATCH -p gu-compute
#SBATCH -A gu-account
#SBATCH --qos=gu-med
#SBATCH --gres=gpu:1
#SBATCH --mem=32G
#SBATCH --time=1:00:00
#SBATCH --output=/home/hozy/torch_setup/logs/pytorch_build_%j.log

echo "==================================="
echo "Environment Check for Build PyTorch"
date
echo "Job running on node: $(hostname)"
echo "==================================="

# Activate Conda environment
CONDA_ENV_NAME="torch_blackwell_default"
echo "Attempting to activate Conda env: $CONDA_ENV_NAME"
source ~/miniconda3/bin/activate $CONDA_ENV_NAME

echo "[DEBUG] Conda env check:"
echo "CONDA_DEFAULT_ENV: $CONDA_DEFAULT_ENV"
echo "[DEBUG] Python path check:"
type python
echo "==================================="

echo "[DEBUG] System Library path check:"
export SYSTEM_LIB_PATH="/usr/lib/x86_64-linux-gnu"
cd $SYSTEM_LIB_PATH || { echo "ERROR: Could not find $SYSTEM_LIB_PATH . Exiting."; exit 1; }
echo "[DEBUG] Current directory check:"
pwd
echo "[DEBUG] Checking for essential NVIDIA driver files..."
ls -l libcuda.so* libnvidia-ml.so* 2>/dev/null
echo "==================================="

echo "Enforcing GLIBC/OpenMP compatibility..."
export LD_PRELOAD=$CONDA_PREFIX/lib/libgomp.so:$CONDA_PREFIX/lib/libstdc++.so:$LD_PRELOAD
echo "[DEBUG] LD_PRELOAD set to: $LD_PRELOAD"
echo "------------------------------------"


export CONDA_LIB_PATH="$CONDA_PREFIX/lib"
export LD_LIBRARY_PATH="$SYSTEM_LIB_PATH:$CONDA_LIB_PATH:$LD_LIBRARY_PATH"

echo "[DEBUG] LD_LIBRARY_PATH set to: $LD_LIBRARY_PATH"
echo "==================================="

echo "Running NVIDIA System Management Interface (nvidia-smi)..."
nvidia-smi
echo "-----------------------------------"

echo "Checking NVCC Compiler Path:"
type nvcc
echo "-----------------------------------"

echo "Check Conda installed packages for CUDA toolkit"
conda list | grep cuda-toolkit
echo "==================================="

PYTORCH_DIR="/scratch/tmp/hozy/torch_setup/pytorch"
echo "Attempting to change directory to: $PYTORCH_DIR"
cd $PYTORCH_DIR || { echo "ERROR: Could not find $PYTORCH_DIR. Exiting."; exit 1; }

echo "[DEBUG] Current directory check:"
pwd

echo "[DEBUG] Directory contents check :"
ls -ld setup.py

echo "==================================="
echo "Fin."
date