# Miniconda3 setup
- Download Miniconda
  ```bash
  wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
  ```
- Install
  ```bash
  bash Miniconda3-latest-Linux-x86_64.sh
  ```
- Create a virtual environment
  ```bash
  conda create -n <env_name> python=<python_version>
  ```
  - e.g.)
    ```bash
    conda create -n min_mdlm python=3.12
    ```


### Torch setup
- Activate the virtual environment
  ```bash
  conda activate <env_name>
  ```
- Install following packages
  ```bash
  pip install cmake 
  ```
  ```bash
  conda install pyyaml
  ```
  ```bash
  conda install cuda-toolkit -c nvidia
  ```
  ```bash
  conda install make c-compiler cxx-compiler
  ```
  ```bash
  pip install six
  ```
- Clone the official PyTorch github repo
  ```bash
  git clone https://github.com/pytorch/pytorch.git
  ```
- Two Options
  - [Install in CLI](#install-in-cli)
  - [Run script](#run-script)

<br>

#### Install in CLI
- Connect the GPU
  ```bash
  srun -p gu-compute -A gu-account --qos=gu-med --gres=gpu:1 --mem=64G --time=4:00:00 --pty bash
  ```
  - Set 4hrs. Installation takes more than an hour.
- Go to the cloned PyTorch repository and run `setup.py`
  ```bash
  pip install . -v
  ```
  - If you failed, clean the repo with below and run the above again.
    ```bash
    git clean -xdf
    ```


#### Run script
- Create environment check script : 
```bash
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


echo "Attempting to activate Conda env: min_mdlm"
source ~/miniconda3/bin/activate min_mdlm

echo "[DEBUG] Conda env check:"
echo "CONDA_DEFAULT_ENV: $CONDA_DEFAULT_ENV"
echo "[DEBUG] Python path check:"
which python
echo "==================================="

PYTORCH_DIR="/home/hozy/torch_setup/pytorch"
echo "Attempting to change directory to: $PYTORCH_DIR"
cd $PYTORCH_DIR || { echo "ERROR: Could not find $PYTORCH_DIR. Exiting."; exit 1; }

echo "[DEBUG] Current directory check:"
pwd

echo "[DEBUG] Directory contents check :"
ls -l setup.py

echo "==================================="
echo "Fin."
date
```
- Run script and check log
  ```bash
  sbatch [script_file]
  ```
- Create actual build script
```bash
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


echo "Attempting to activate Conda env: min_mdlm"
source ~/miniconda3/bin/activate min_mdlm

echo "[DEBUG] Conda env check:"
echo "CONDA_DEFAULT_ENV: $CONDA_DEFAULT_ENV"
echo "[DEBUG] Python path check:"
which python
echo "==================================="

PYTORCH_DIR="/home/hozy/torch_setup/pytorch"
echo "Attempting to change directory to: $PYTORCH_DIR"
cd $PYTORCH_DIR || { echo "FATAL ERROR: Could not find $PYTORCH_DIR. Exiting."; exit 1; }

echo "[DEBUG] Current directory check:"
pwd

echo "[DEBUG] Directory contents check :"
ls -l setup.py

echo "==================================="

echo "Clean PyTorch repo."
git clean -xdf

echo "Start building"
pip install . -v

echo "Fin."
date
```
- Run script
  ```bash
  sbatch [script_file]
  ```
- Check status
  ```bash
  squeue -u [user_name]
  ```
