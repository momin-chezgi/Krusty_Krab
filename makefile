
SRCS := \
	src/Domain/Admin.cpp \
	src/Domain/Customer.cpp \
	src/Domain/Drink.cpp \
	src/Domain/Food.cpp \
	src/Domain/Menu.cpp \
	src/Domain/MenuItem.cpp \
	src/Domain/Order.cpp \
	src/Domain/Restaurant.cpp \
	src/Domain/Restaurateur.cpp \
	src/Repository/AdminStorage.cpp \
	src/Repository/CustomerStorage.cpp \
	src/Repository/MenuStorage.cpp \
	src/Repository/OrderStorage.cpp \
	src/Repository/RestaurantStorage.cpp \
	src/Repository/RestaurateurStorage.cpp \
	src/Repository/SQL.cpp \
	src/UI/GetInformation.cpp \
	src/UI/Interface.cpp \
	src/UI/Printer.cpp \
	src/Utility/IDGenerator.cpp \
	src/main.cpp

krusty_krab: $(SRCS)
	g++ -std=c++14 -Wall -Wextra -Iinclude $(SRCS) -o krusty_krab
