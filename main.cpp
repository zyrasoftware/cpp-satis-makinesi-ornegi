#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

class Product {
public:
    Product(int id, const std::string& name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

    int getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    int getQuantity() const {
        return quantity;
    }

    void reduceQuantity() {
        if (quantity > 0)
            quantity--;
    }

private:
    int id;
    std::string name;
    double price;
    int quantity;
};

class VendingMachine {
public:
    VendingMachine(const std::string& productFile) {
        loadProducts(productFile);
        balance = 0.0;
    }

    void displayMenu() const {
        std::cout << "----- Otomatik Satış Makinesi -----" << std::endl;
        std::cout << "Bakiye: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "Ürünler:" << std::endl;

        for (const auto& product : products) {
            std::cout << product.getId() << ". " << product.getName() << " - $" << product.getPrice();
            if (product.getQuantity() == 0)
                std::cout << " (Stokta yok)";
            std::cout << std::endl;
        }

        std::cout << "----------------------------------" << std::endl;
    }

    void insertMoney(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Bakiyeniz: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        }
        else {
            std::cout << "Geçersiz miktar." << std::endl;
        }
    }

    void purchaseProduct(int productId) {
        for (auto& product : products) {
            if (product.getId() == productId) {
                if (product.getQuantity() > 0 && balance >= product.getPrice()) {
                    balance -= product.getPrice();
                    product.reduceQuantity();
                    std::cout << product.getName() << " satın alındı. Kalan bakiye: $" << std::fixed << std::setprecision(2) << balance << std::endl;
                }
                else if (product.getQuantity() == 0) {
                    std::cout << "Üzgünüz, bu ürün tükenmiştir." << std::endl;
                }
                else {
                    std::cout << "Yetersiz bakiye. Bakiyeniz: $" << std::fixed << std::setprecision(2) << balance << std::endl;
                }
                return;
            }
        }

        std::cout << "Geçersiz ürün ID'si." << std::endl;
    }

private:
    std::vector<Product> products;
    double balance;

    void loadProducts(const std::string& productFile) {
        std::ifstream file(productFile);

        if (!file.is_open()) {
            std::cerr << "Ürün dosyası bulunamadı." << std::endl;
            return;
        }

        int id, quantity;
        std::string name;
        double price;

        while (file >> id >> name >> price >> quantity) {
            products.emplace_back(id, name, price, quantity);
        }

        file.close();
    }
};

int main() {
    setlocale(LC_ALL, "Turkish");
    VendingMachine vendingMachine("products.txt");

    int choice;
    double amount;

    do {
        vendingMachine.displayMenu();

        std::cout << "1. Para Ekle" << std::endl;
        std::cout << "2. Ürün Satın Al" << std::endl;
        std::cout << "0. Çıkış" << std::endl;
        std::cout << "Seçiminizi yapın: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Eklemek istediğiniz miktarı girin: $";
            std::cin >> amount;
            vendingMachine.insertMoney(amount);
            break;
        case 2:
            std::cout << "Satın almak istediğiniz ürünün ID'sini girin: ";
            std::cin >> choice;
            vendingMachine.purchaseProduct(choice);
            break;
        case 0:
            std::cout << "Çıkış yapılıyor..." << std::endl;
            break;
        default:
            std::cout << "Geçersiz seçenek." << std::endl;
        }

    } while (choice != 0);

    return 0;
}
