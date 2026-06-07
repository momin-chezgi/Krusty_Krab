# C++ Mini-Project

_Date: May 14, 2026 / Ordibehesht 24 1405_

> Note: All of the Interface will be implemented in CLI (not in GUI)

## Steps:
* Login page: 
    1. Choose your position/rule
* If user is a customer:
    1. Sees the list of active restaurants (with the name, address, average preparation time of any of them)
    2. Chooses one of the restaurants
    3. Sees the restaurant menu: name, bio, price, is available
    4. User chooses each item if wants 
    5. In each picking up of an item the total cost of the order should be shown
    6. User finalizes the order
    7. The order id and the current status is shown
    8. Even the customer can see the history of the orders and follow that is prepared or not
* If user is a restaurateur:
    1. Enters the id of the restaurant
    2. After entering to the dashboard, the informations of the restaurant: name, address, status and the average preparation time would be presented
    3. The restaurateur can modify the informations of the restaurant : Changes the prices, bio, availability status of an item
    4. Even can remove an item from the menu
    5. The restaurateur accesses to the current orders. modify, remove or change the status of the order: In-preparation, ready-to-send and delivered
    
* If user is a system admin:
    0. [optional] Needs a password to log-in
    1. Admin can create a new restaurant with all of the needed details, activates and disactivates a restaurant(s) and recieves the statistics