> In the Name of Allah

# Krusty Krab restaurant management system

> If you are a random guy on GitHub who is reading this README, I recommend you leave out! because it is just a boring university project.

The name of the project was inspired by the `SpongeBob SquarePants` series!

<img src='krusty_krab.jpeg' width=&quot400&quot height=&quot400&quot>

This is a CLI-based project for simulating a restaurant management system. The programming language is C++14. 
The base of the UI is prompting, like most CLI commands.
There are three roles:
I) Customer
II) Restaurateur(Manager)
III) Admin of the system
We also have some other characteristics:
* Restaurant
* Order
* Menu
* Menu Item (contains food and drink)
* Cart (when order hasn't been finalised)

Each level of user has its specific access:
### For Customer:
1. Placing an order
2. Tracing its orders

### For Manager:
1. Monitoring statistics of the restaurant
2. Changing its details and the restaurant details
3. Add/remove an item from the restaurant menu
4. Add/remove an order from the queue of orders
5. changing the status of an order(In-Preparation, Ready-to-send, delivered or cancelled)

### For Admin:
1. Add/delete a manager account
2. Create a restaurant
3. Active/deactivate a restaurant
4. monitoring total statistics

