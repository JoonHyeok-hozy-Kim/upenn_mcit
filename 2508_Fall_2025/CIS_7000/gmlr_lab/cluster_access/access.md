### SSH
- login1
  ```bash
  ssh hozy@grasp-login1.seas.upenn.edu
  ```
- login2
  ```bash
  ssh hozy@grasp-login2.seas.upenn.edu
  ```

<br>

### GPU Request
```bash
srun -p gu-compute -A gu-account --qos=gu-med --gres=gpu:1 --mem=32G --time=4:00:00 --pty bash
```
