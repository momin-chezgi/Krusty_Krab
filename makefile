
SRCS := \
	./src/Management/Admin.cpp   		\
	./src/Management/Customer.cpp   		\
	./src/Management/Food.cpp   		\
	./src/Management/Drink.cpp   		\
	./src/Management/Menu.cpp   		\
	./src/Management/MenuItem.cpp   		\
	./src/Management/Order.cpp   		\
	./src/Management/Restaurant.cpp   		\
	./src/Management/Restaurateur.cpp   		\
	src/Utils/IDGenerator.cpp  \
	src/IO/Interface.cpp  \
	src/main.cpp

krusty_krab: $(SRCS)
	g++ -std=c++14 -Wall -Wextra -Iinclude $(SRCS) -o krusty_krab	src/Management/FoodItem.cpp  \

