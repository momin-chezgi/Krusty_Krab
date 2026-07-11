# Database Structure and Dynamic Storage Design

This project uses a local SQLite database as the persistence layer. The data layer is initialized by DatabaseManager and every domain/repository operation goes through storage classes (DAO-like classes).

## Storage engine and startup flow

1. DatabaseManager opens the database file data/krusty_krab.db.
2. It enables foreign keys and runs bootstrap.
3. Bootstrap creates all tables if missing and inserts seed records using INSERT OR IGNORE.
4. All DB commands use prepared statements. Transactions are used when multiple statements must stay consistent.

## Core tables

### Admin and ownership
- admins: id primary key, name
- admin_restaurateurs: admin_id, restaurateur_id, composite primary key (admin_id, restaurateur_id)

### Customers and restaurateurs
- customers: id primary key, name
- restaurateurs: id primary key, restaurant_id, name

### Menus and restaurants
- menus: id primary key
- menu_items: id, menu_id, item_type, name, bio, price, stock_quantity, preparation_minutes, food_type
- restaurants: id, menu_id, name, address, active, preparation_minutes, phone, bio

### Orders
- orders: id, orderer_id, restaurant_id, status, total_price
- customer_orders: customer_id, order_id, composite primary key
- restaurant_orders: restaurant_id, order_id, composite primary key
- order_items: order_id, item_id, item_type, name, bio, price, quantity, quantity_snapshot, preparation_minutes, food_type

### Membership (phase 2)
- membership_levels: customer_id, level, starting_point, ending_point, current_points, multiplier, off_percentage, delivery_discount, lottery_ticket
- membership_level_logs: customer_id, changed_at, level, points

## How dynamic data is stored

- Dynamic writes happen immediately through storage calls, not by directly changing in-memory maps.
- When a customer loads:
  - Customer object comes from customers table.
  - Related order IDs come from customer_orders.
  - Membership state is loaded from membership_levels.
- When creating an order:
  - A row is inserted into orders.
  - Each line is inserted into order_items.
  - Relationships are inserted into customer_orders and restaurant_orders.
- Dynamic update example:
  - Changing restaurant status/fields updates the restaurants table.
  - Adding/removing menu items updates menu_items rows and the owning restaurant menu linkage.
  - Placing an order updates order_status and totals in orders.
- Membership updates are dynamic:
  - Current status is updated in membership_levels.
  - Every change is also appended to membership_level_logs for history.

## Why this is dynamic and not static

- Data persists across runs in database files.
- IDs are used as stable references across tables.
- Bridge tables (customer_orders, restaurant_orders) represent changing relations.
- Every run reads latest state from DB, so user/admin actions are always reflected.
