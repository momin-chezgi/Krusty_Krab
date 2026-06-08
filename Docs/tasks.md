### Essentials:
1. Document: drawing the pattern of the classes
2. ~~Implement id assignment for each class~~ -> ID generation is the same all around the code (see at the very end : 2)
3. ~~List all the types of food (done)~~

Learn...
1. ~~safe pointers~~
2. maps(done)
3. default constructors (done)
4. 'cast' types in modern C++
5. anonymous namespaces
6. deeply about streams
7. static functions
8. lambda functions
9. Learning ~~MySQL~~ -> SQLite

In Customer:
1. Dashboard: show the list of all active restaurants
2. Exit & Delete: place an option 'delete account' by entering y

In Restaurant:
1. Menu: print 'weight' or 'volume' instead of 'quantity'
2. Menu: print the quantity for each item
3. Dashboard: contains an option for showing complete details

In Menu:
1. Implement quantity

In Order:
1. Check while placing order if exists at least the requested quantity in the menu

In Restaurateur:
1. Presenting: Show menu, queue, etc
2. Order: edit orderStatus
3. Order: delete the unnecessary 'replace' function, because you can use delete/add instead of
4. Menu: delete the unnecessary 'replace' function

Common:
1. input: get a line for strings instead of a raw cin (done)



### Optionals:
1. Design a cancel choice for each order and item
2. Password: have this system to be more secure by passwords and access-controlers
3. set instead of vector: Use set instead of vector in places you want not to store an ID in two times
4. Add an clear() option in command line
5. Don't get stuck in a loop, if the ID isn't correct, show an error message and get out.
6. Between two followed lines, use an empty line at least



### Steps:
1. Create all the prototypes of needed classes in header files. (almost done)
2. Write interface/ completely(Interface -> Getinf -> Printer), You can do it at the end if the time seems short
3. ~~Code (or maybe redesign) completely FoodTypes/ (The foundations are typed)~~
4. ~~Code (or maybe redesighn) completely Container/ (The foundations are typed)~~
5. Code (or maybe change sths) MenuItem/ (The foundations are typed)
6. Code Menu/ (The foundations are typed)
7. Code Order/ (The foundations are typed)
8. Code Restaurant/ (The foundations are typed)
9. Code Restaurateur/ (The foundations are typed)
10. Code Admin/ (The foundations are typed)
... And It's time for InOut/ and ~~MySQL~~ SQLite for data storage
... Maybe after finishing InOut/ it's vital to redesign and rewrite the codes !
11. Think how to deal with statistics 
12. ID Management: hide, change them to string(from size_t) and use UUID(Done!)



### At the very end:
0. Use ~~MySQL~~ SQLite
1. Add the option of creating a user
2. Create UUID (done)
3. Implement repositories (done)
4. Organize all the files in a well-organized form(like domain/ headers/ repository/ etc.) (done)
5. Use namespaces for each logical space
6. Use smart pointers
7. Use Cpp modern casting
8. Add operator overloadings (done)
9. Catch errors
