import os

if __name__ == '__main__':

    os.chdir(r'C:\Users\danie\git_hub\upenn_mcit\2408_Fall_2024\CIT_592\HW\01')
    with open("d.txt", 'r', encoding="UTF-8") as file:
        data = file.readlines()
    
    cnt = 0
    for l in data:
        splitted = l.split("&")[0]
        print(splitted, end="")
        if cnt %3 == 2:
            print("\\\\")
        else:
            print("&")
        cnt+=1
