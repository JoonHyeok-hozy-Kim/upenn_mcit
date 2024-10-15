import os
from datetime import datetime, timedelta

def search_and_delete_old_items(dir):
    for root, dirs, files in os.walk(dir):
        for file in files:
            if is_old(file):
                path = os.path.join(dirs, file)
                # print("remove : {}".format(path))
                os.remove(path)

def is_old(file_name):
    hyphen_sep = file_name.split("-")
    yyyy, mm, dd = None, None, None
    for str in hyphen_sep:
        if yyyy and mm and dd:
            break
        if len(str) == 4 and str[0].isdigit() and str[1].isdigit() and str[2].isdigit() and str[3].isdigit():
            yyyy = str
        elif len(str) == 2 and yyyy is not None and mm is not None and str[0].isdigit() and str[1].isdigit():
            dd = str
        elif len(str) == 2 and yyyy is not None and str[0].isdigit() and str[1].isdigit():
            mm = str
        else:
            yyyy, mm, dd = None, None, None
            
    if yyyy is None or mm is None or dd is None:
        return False
    
    ninety_from_today = datetime.today() - timedelta(days=90)
    parsed_date = datetime(int(yyyy), int(mm), int(dd))

    if parsed_date < ninety_from_today:
        return True
    
    return False



if __name__ == '__main__':
    D = "IV_Prep/HZ/xtern/cleaning-up-old-files"
    search_and_delete_old_items(D)