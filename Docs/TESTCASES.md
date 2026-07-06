Scenario 1: Order placement then automatic membership upgrade happens:
~~~
========================================================
Customer Dashboard
========================================================
Welcome, S1
Membership level: Normal Member
Current points: 0
Points needed for next level: 100
Order discount: 0%
Delivery benefit: No delivery discount
Lottery tickets: 0
...
Select an option: 1
Choose a restaurant by entering its ID:
...
Restaurant Menu
========================================================
  - Krusty Cola ... ID: TestDrink
  - Krabby Patty ... ID: TestFood
Would you like to place an order? (y/n)
y
Enter item ID to add to your cart (q to finish):
TestFood
Enter the weight (kg): 11
Current total price: 275
Enter item ID to add to your cart (q to finish): q

========================================================
Checkout Invoice
========================================================
Base total: 275
Membership discount (0%): -0
Delivery base fee: 10
Delivery discount: -0
Final delivery fee: 10
Final payable total: 285
Points earned from this order: 110

Congratulations! Your membership level increased from Normal to Silver.
Order successfully created. Order ID: <uuid>
~~~
Scenario 2: VIP user gets discount has free delivery
~~~
Enter admin ID:
...
Select an option: 42
Enter new membership level (0=Normal, 1=Silver, 2=Gold, 3=VIP): 3
Enter membership points: 700
Membership updated for customer: TestCustomer

========================================================
Customer Dashboard
========================================================
Welcome, TestCustomer
Membership level: VIP Member
Current points: 700
Points needed for next level: top level reached
Order discount: 15%
Delivery benefit: Free delivery
Lottery tickets: 3
...
Would you like to place an order? (y/n) y
...
Enter item ID to add to your cart: TestFood
Enter the weight (kg): 1
Current total price: 25
...
========================================================
Checkout Invoice
========================================================
Base total: 25
Membership discount (15%): -3.75
Delivery base fee: 10
Delivery discount: -10
Final delivery fee: 0
Final payable total: 21.25
Points earned from this order: 20
Order successfully created. Order ID: <uuid>
~~~
Scenario 3: Admin changes one customer’s membership level
~~~
========================================================
Admin Dashboard
========================================================
Membership management:
41. Print membership level report
42. Change a customer's membership
43. Show a customer's membership level history
...
Select an option: 42

Enter new membership level (0=Normal, 1=Silver, 2=Gold, 3=VIP): 3
Enter membership points: 700
Membership updated for customer: TestCustomer
~~~