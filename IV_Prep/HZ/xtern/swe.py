import datetime

permissions = [
    { "role": "superuser", "name": "lock user account", "active": True },
    { "role": "superuser", "name": "unlock user account", "active": True },
    { "role": "superuser", "name": "purchase widgets", "active": False },
    { "role": "charger", "name": "view pick up locations", "active": True },
    { "role": "rider", "name": "view my profile", "active": True },
    { "role": "rider", "name": "scooters near me", "active": True },
]

users = [
    { "id": 1, "name": "Anna Administrator", "roles": ["superuser"] },
    { "id": 2, "name": "Charles N. Charge", "roles": ["charger", "rider"] },
    { "id": 7, "name": "Ryder", "roles": ["rider"] },
    { "id": 11, "name": "Unregistered Ulysses", "roles": [] },
    { "id": 18, "name": "Tessa Tester", "roles": ["beta tester"] },
]

D = datetime.timedelta(hours=3)
print(round(D.seconds/60/60))

print("$")

x = 13.43435
print(round(x,2))