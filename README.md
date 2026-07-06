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
* Cart (when the order hasn't been finalised)

Each level of user has its specific access:
### For Customer:
1. Browse restaurants and select an active restaurant.
2. Browse restaurant menus (food and drink items).
3. Build an order with quantities and create it.
3. Track order readiness and view order history.
4. Observe membership-level behaviour where applicable.

### For Manager:
1. Monitoring statistics of the restaurant
2. Edit restaurant profile details.
3. Changing its details and the restaurant details
4. Add/remove an item from the restaurant menu
5. Adjust stock levels.
6. Add/remove an order from the queue of orders
7. updating the status of an order(In-Preparation, Ready-to-send, delivered or cancelled)

### For Admin:
1. Add/delete a manager account
2. Create a restaurant
3. Active/deactivate a restaurant
4. Monitoring total statistics

## Project structure

- `include/Domain` and `src/Domain`: entities and business logic.
- `include/Repository` and `src/Repository`: static storage and SQL-oriented classes.
- `include/UI` and `src/UI`: command-line interaction and user-flow orchestration.
- `include/Utility` and `src/Utility`: shared helpers and ID generation utilities.
- `Docs`: design notes and references.

## Storage strategy

The current implementation is a transitional prototype.

1. Some flows operate through in-memory repository maps for direct lightweight behaviour.
2. SQLite-backed repositories are integrated to support persistence-oriented development.
3. IDs are string-based, with hydration and entity reconstruction currently evolving around repository boundaries.

## Command-line workflow

1. Start the application.
2. Choose a role.
3. Provide input-driven data for that role flow.
4. Navigate the role menu to perform operations.
5. Return to menus or exit cleanly.

> This README reflects the current shape of the project and should evolve with each development phase.
