#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>


void clearScreen();

class Ornament {
    std::string name;
    float amount;
    float price; // per bucata

public:
    Ornament(std::string  n, const float amnt, const float p ) : name(std::move(n)), amount(amnt), price(p) {}

    Ornament(const Ornament& other) =default;

    Ornament& operator=(const Ornament& other) {
        if (this != &other) {
            name = other.name;
            amount = other.amount;
            price = other.price;
        }
        return *this;
    }

    [[nodiscard]] const std::string& getName() const& {
        return name;
    }

    [[nodiscard]] float getAmount() const {
        return amount;
    }

    [[nodiscard]] float getPrice() const {
        return price;
    }

    ~Ornament()=default;

    friend std::ostream& operator<<(std::ostream& os, const Ornament& ornm) {
        os << ornm.name << " (" << ornm.amount << " grams)";
        return os;
    }
};

class CakeDetails {
    std::string CakeName;
    std::string flavor;
    std::string cream;
    std::string message;
    std::vector<Ornament> ornaments;
    int tiers;
    int days_until_expiration;
    float weight_without_ornaments;
    float price_per_kg;
    bool sugarfree;
    bool glutenfree;

public:
    CakeDetails(const std::string  &cknm, const std::string& flv, std::string cr, const std::string& msg,
             const std::vector<Ornament>& ornm, const int t, const int days, const float w, const float price , const bool nosugar, const bool nogluten)
    : CakeName(cknm), flavor(flv), cream(std::move(cr)), message(msg), ornaments(ornm), tiers(t),
      days_until_expiration(days), weight_without_ornaments(w), price_per_kg(price), sugarfree(nosugar), glutenfree(nogluten) {}

    [[nodiscard]] bool isSugarfree() const {
        return sugarfree;
    }

    [[nodiscard]] bool isGlutenfree() const {
        return glutenfree;
    }

    [[nodiscard]] const std:: string & getCakeName() const {
        return CakeName;
    }

    [[nodiscard]] const float  & getPrice() const {
        return price_per_kg;
    }
    [[nodiscard]] const float  & getWeight() const {
        return weight_without_ornaments;
    }
    [[nodiscard]]float TotalWeight( float  total_weight) const {
        for (const auto& ornm : ornaments) {
            total_weight += ornm.getAmount();
        }
        return total_weight;
    }

    [[nodiscard]]float CostOfCake(float price) const {
        for (const auto& ornm : ornaments) {
            price += ornm.getPrice();
        }
        return price;
    }

    void displaySummary() const {
        std::cout << "Flavor of the cake base: " << this->flavor << std::endl;
        std::cout << "Number of tiers: " << this->tiers << std::endl;
        std::cout << "Weight of the cake (edible): " << this->weight_without_ornaments << " killograms" << std::endl;
        std::cout << "Covering cream: " << this->cream << std::endl;
        std::cout << "Price per kg: " << this->price_per_kg << std::endl;
        std::cout << "Days until expiration: " << this->days_until_expiration << std::endl;
        std::cout << "Message on the cake(can be changed if requested) : " << this->message << std::endl;
        std::cout << "Ornaments (on top of cake): ";
        for (const auto& ornm : ornaments) {
            std::cout << ornm.getName() << " "<<std::endl;
        }
        std::cout <<"\n";
        std::cout << "Does it contain sugar? " << (isSugarfree() ? " no" : " yes") << "\n";
        std::cout << "Is it gluten free? " << (isGlutenfree() ? " yes\n" : " no\n") << "\n";
        std::cout << "All cakes have carefully chosen details that add to the weight and the price of the cake. Here are some more details: \n\n";
        std::cout << "Total weight for the cake is: " << TotalWeight(this->weight_without_ornaments ) << " killograms "<< std::endl;
        std::cout << "Final price for the cake is: " << CostOfCake(this->weight_without_ornaments * this->price_per_kg) << " lei "<< std::endl;
    };

    friend std::ostream& operator<<(std::ostream& os, const CakeDetails& details) {
        os << "Cake Name: " << details.CakeName << "\n"
           << "Flavor: " << details.flavor << "\n"
           << "Cream: " << details.cream << "\n"
           << "Message: " << details.message << "\n"
           << "Tiers: " << details.tiers << "\n"
           << "Days Until Expiration: " << details.days_until_expiration << "\n"
           << "Weight (without ornaments): " << details.weight_without_ornaments << " kg\n"
           << "Price per kg: " << details.price_per_kg << " lei\n"
           << "Sugar-Free: " << (details.sugarfree ? "Yes" : "No") << "\n"
           << "Gluten-Free: " << (details.glutenfree ? "Yes" : "No") << "\n"
           << "Ornaments: ";
        for (const auto& ornm : details.ornaments) {
            os << ornm << " ";
        }
        os << "\n";
        return os;
    }
};


class Cake {

    CakeDetails details;

public:
    explicit Cake(const CakeDetails& custom) : details(custom) {}

    [[nodiscard]]const CakeDetails& getCakeDetails() const { return details; }

    friend std::ostream& operator<<(std::ostream& os, const Cake& cake) {
        os << cake.details; // This will now use CakeDetails's overloaded operator<<
        return os;
    }
};



class Order {
    int orderID;
    static int orderCounter;
    CakeDetails cake;
    std::string DateOfDelivery;
    std::string payment;

public:
    Order(const CakeDetails& ck, std::string Delivery, std::string pay)
    : cake(ck), DateOfDelivery(std::move(Delivery)), payment(std::move(pay)) {
        orderID = ++orderCounter;
    }

    [[nodiscard]] int getOrderID() const {
        return orderID;
    }

    [[nodiscard]] const CakeDetails& getCakeDetails() const {
        return cake;
    }

    [[nodiscard]] const std::string& getDateOfDelivery() const {
        return DateOfDelivery;
    }

    [[nodiscard]] const std::string& getPayment() const {
        return payment;
    }

    ~Order() =default;


    friend std::ostream& operator<<(std::ostream& os, const Order& order){
        os << "Order ID: " << order.getOrderID() << " Cake: ";
        order.getCakeDetails().displaySummary();
        os << " Delivery date: " << order.getDateOfDelivery() << " Payment method: " << order.getPayment() << std::endl;
        return os;
    }
};

class Client {

    Order order;
    std::string name;
    std::string phoneNumber;
    std::string OrderAddress;
    std::string EmailAddress;

public:
    Client( const Order &ord, std::string nm, std::string phone, std::string adrs, std::string email)
    : order(ord), name(std::move(nm)), phoneNumber(std::move(phone)), OrderAddress(std::move(adrs)), EmailAddress(std::move(email)) {}

    void placeOrder(const std::vector<CakeDetails>& cakes) {
        std::cout << "*********************************************************\n"
                  << "Thank you for taking an interest in Sweet Spell Bakery! \nWe make cakes out of love and passion\n"
                  << "*********************************************************\n\n"
                  << "Our current cake collection is:\n\n"
                  << "*********************************************************\n"
                  << "\n";

        int aux = 1;
        for (const auto& c : cakes) {
            std::cout << "Option " << aux << ": " << c.getCakeName() << "\n";
            aux++;
        }

        std::cout << "*********************************************************\n"
                  << "\n\n"
                  << "Which one would you like to order? (choose just the number <3): ";

        const int order_temp = getValidCakeSelection(cakes); // Using the helper function

        clearScreen();

        std::cout<<"---------------------------------------------------------\n"
                 <<"*********************************************************\n"
                 <<"\n     Amazing choice! It's one of our top sellers! :)\n\n"
                 << "*********************************************************\n"
                 <<"---------------------------------------------------------\n";

        std::this_thread::sleep_for(std::chrono::seconds(3));

        clearScreen();

        cakes[order_temp - 1].displaySummary();

        //std::this_thread::sleep_for(std::chrono::seconds(10));
        //clearScreen();


        // Collect order details
        std::string deliveryDate;
        std::cout << "\nEnter delivery date (format: MM-DD): ";
        std::cin >> deliveryDate;

        int aux_paymentMethod;
        std::string paymentMethod;
        std::cout << "\nPlease choose a payment method  (Choose just the number)\n"
                  << "(" << cakes[order_temp - 1].CostOfCake(   cakes[order_temp - 1].getPrice() * cakes[order_temp - 1].getWeight()  )<<") ron\n"
                  << "Option 1: Cash on Delivery\n"
                  << "Option 2: Online Payment by Card\n"
                  << "Option 3: Bank Deposit\n";
        std::cin >> aux_paymentMethod;

        while (true) {
            if (aux_paymentMethod == 1 || aux_paymentMethod == 2|| aux_paymentMethod == 3) {
                if (aux_paymentMethod == 1) {
                    paymentMethod = "Cash on Delivery";
                    std::cout << "You chose Cash on Delivery as payment method.\n";
                    break;
                } if (aux_paymentMethod == 2) {
                    paymentMethod = "Online Payment by Card";
                    std::cout << "You chose Online Payment by Card.\n";
                    break;
                }  if (aux_paymentMethod == 3) {
                    paymentMethod = "Bank Deposit";
                    std::cout << "You chose Bank Deposit.\n";
                    break;
                }
            }else {
                clearScreen();
                std::cout << "\n ****** Invalid input.******  \n"
                <<"Please select a valid paying method from 1 to 3: "
                << "Option 1: Cash on Delivery\n"
                << "Option 2: Online Payment by Card\n"
                << "Option 3: Bank Deposit\n";
                std::cin >> aux_paymentMethod;
            }
        }

        const Order newOrder( cakes[order_temp - 1], deliveryDate, paymentMethod );

        this->order = newOrder;

        std::cout << "\nOrder successfully placed! Thank you for supporting our business!\n";
    }

    static int getValidCakeSelection(const std::vector<CakeDetails>& cakes) {
        int order_temp = 0;
        std::cin >> order_temp;

        while (order_temp < 1 || order_temp > static_cast<int>(cakes.size())) {
            std::cout << "\nInvalid selection. Please enter a number between 1 and " << cakes.size() << " :)\n";
            std::cin >> order_temp;
        }

        return order_temp;
    }


    friend std::ostream& operator<<(std::ostream& os, const Client& cust) {
        os << "Customer: " << cust.name << ", Phone: " << cust.phoneNumber << " Email: " << cust.EmailAddress << " Address: " << cust.OrderAddress << std::endl;
        return os;
    }
};

int Order::orderCounter = 0;

void displayWelcomeMessage(  ) {
    std::string line1 = "        Welcome to";
    std::string line2 = "    Sweet Spell Bakery!";
    constexpr int delay = 300;

    std::cout << "---------------------------------------------------------\n";
    std::cout << "*********************************************************\n";
    std::cout << "*                ";
    std::string word = "";

    for (const char caracter : line1) {
        if (caracter == ' ') {
            std::cout << word << " ";
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            word.clear();  // reset word for the next
        } else {
            word += caracter; }
    }
    std::cout << word << "                     *\n";
    std::cout << "*                ";
    word = "";

    for (const char caracter : line2) {
        if (caracter == ' ') {
            std::cout << word << " ";
            std::cout.flush();
            std::this_thread::sleep_for( std::chrono::milliseconds(delay) );
            word.clear();
        } else {
            word += caracter;
        }
    }
    std::cout << word << "                *\n";

    std::cout    << "---------------------------------------------------------\n"
                 << "*********************************************************\n"
                 << "*                                                       *\n"
                 << "*                        *   *   *                      *\n"
                 << "*                        |   |   |                      *\n"
                 << "*                     ~~~~~~~~~~~~~~~                   *\n"
                 << "*                    |we all love cake|                 *\n"
                 << "*                    ******************                 *\n"
                 << "*                   |     *      *     |                *\n"
                 << "*                  **********************               *\n"
                 << "*                 |      *   * *   *     |              *\n"
                 << "*                 ************************              *\n"
                 << "*                | *       *     *      * |             *\n"
                 << "*                |     *      *     *     |             *\n"
                 << "*********************************************************\n"
                 << "---------------------------------------------------------\n";
}

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

int main() {
     displayWelcomeMessage();

     clearScreen();

     Ornament ornm1("Mrs&Mr Figurine", 0.3f, 100);
     Ornament ornm2("Boy/Girl Figurine", 0.3f, 20 );
     Ornament ornm4("Animated Character Figurine", 0.1f, 30);

     std::vector<Ornament> birthdayOrnaments= { ornm2, ornm4 };
     std::vector<Ornament> weddingOrnaments= { ornm1 };
     std::vector<Ornament> carrotOrnaments= { ornm4 };
     std::vector<Ornament> cheesecakeOrnaments= { ornm4};
     std::vector<Ornament> lemonOrnaments= {ornm2 };
     std::vector<Ornament> redVelvetOrnaments= {ornm2};

    std::vector<CakeDetails> cakes = {
        CakeDetails("Chocolate Birthday Monster", "Chocolate", "Chocolate Ganache", "Happy Birthday (name and age)!",
            birthdayOrnaments, 1, 5, 2.0f, 100, false, false),
        CakeDetails("Vanilla Birthday Lover", "Vanilla", "Buttercream", "Happy Birthday (name and age)!",
            birthdayOrnaments, 1, 5, 2.0f, 100, false, false),
        CakeDetails("Vanilla Wedding Dream", "Vanilla", "White Chocolate Ganache", "For a lifetime of happiness Mrs&Mr",
            weddingOrnaments, 3, 3, 10.0f, 150, false, false),
        CakeDetails("Carrot Cake", "Carrot", "Cream Cheese", "Don't forget to smile!",
            carrotOrnaments, 1, 6, 1.5f, 80, false, true),
        CakeDetails("Classic Cheesecake Sugar&Gluten Free", "Biscuit Base", "Cream Cheese", "Have a beautiful day",
            cheesecakeOrnaments, 1, 5, 1.5f, 80, true, true),
        CakeDetails("Forest Fruit Cheesecake Sugar Free", "Biscuit Base", "Cream Cheese", "Have a beautiful day",
            cheesecakeOrnaments, 1, 5, 1.5f, 90, true, false),
        CakeDetails("Lemon Graduation Cake Sugar&Gluten free", "Vanilla with Lemon", "Lemon Frosting", "Happy Graduation!",
            lemonOrnaments, 1, 4, 2.0f, 90, true, true),
        CakeDetails("Congratulation Special: Red Velvet", "Red Velvet", "Cream Cheese", "Congratulations!",
            redVelvetOrnaments, 2, 5, 3.0f, 130, false, false)
    };

    Client client("John Doe", "123-456-789", "123 Cake St, Sweet City", "john@example.com", Order(cakes[0], "2024-12-25", "Credit Card"));

    client.placeOrder(cakes);

    return 0;
}