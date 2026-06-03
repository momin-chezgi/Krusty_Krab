#pragma once

# include <iostream>
# include <vector>
# include <map>
# include <algorithm>

using namespace std;

typedef double cost;
typedef string ID_tp;
typedef string RestID_tp;
typedef string ManagerID_tp;  // Manager = restaurateur
typedef string ItemID_tp;
typedef string AdminID_tp;
typedef string OrderID_tp;
typedef string CustID_tp;

class MenuItem;

//--------------# IDs #-------------------
//  Plates:
#define Soup_Plate_ID           10
#define Pasta_Plate_ID          11
#define Rice_Plate_ID            12
#define Cheese_Plate_ID       13
#define Appetizer_Plate_ID    14
#define Salad_Plate_ID          15
//  DrinkContainers:
//  Foods:
