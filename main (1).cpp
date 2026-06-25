/*
    Навчальна практика
    Варіант — Менеджер контактів

    Програма дозволяє:
    - додавати контакти;
    - переглядати всі контакти;
    - шукати контакти;
    - редагувати номер телефону;
    - видаляти контакти;
    - зберігати дані у файл.
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

// Видаляє зайві пробіли
std::string trim(const std::string& s) {
    const std::string ws = " \t\r\n";

    size_t start = s.find_first_not_of(ws);

    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(ws);

    return s.substr(start, end - start + 1);
}

// Розбиває рядок на частини
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;

    std::stringstream ss(line);
    std::string item;

    while (getline(ss, item, delimiter)) {
        parts.push_back(trim(item));
    }

    return parts;
}

// Завантаження контактів із файлу
std::vector<Contact> loadContacts(const std::string& filename) {

    std::vector<Contact> contacts;

    std::ifstream file(filename);

    if (!file.is_open())
        return contacts;

    std::string line;

    while (getline(file, line)) {

        auto parts = split(line, '|');

        if (parts.size() != 3)
            continue;

        Contact c;

        c.name = parts[0];
        c.phone = parts[1];
        c.email = parts[2];

        contacts.push_back(c);
    }

    return contacts;
}

// Збереження контактів у файл
void saveContacts(const std::string& filename,
                  const std::vector<Contact>& contacts) {

    std::ofstream file(filename);

    for (const auto& c : contacts) {

        file << c.name << "|"
             << c.phone << "|"
             << c.email << "\n";
    }
}

// Зчитування непорожнього рядка
std::string readString(const std::string& prompt) {

    while (true) {

        std::cout << prompt;

        std::string value;

        getline(std::cin, value);

        value = trim(value);

        if (!value.empty())
            return value;

        std::cout << "Поле не може бути порожнім.\n";
    }
}

// Виведення одного контакту
void printContact(int index, const Contact& c) {

    std::cout << index << ". "
              << c.name
              << " | "
              << c.phone
              << " | "
              << c.email
              << "\n";
}

// Додавання нового контакту
void addContact(std::vector<Contact>& contacts) {

    std::cout << "\n--- Додавання контакту ---\n";

    Contact c;

    c.name = readString("Ім'я: ");
    c.phone = readString("Телефон: ");
    c.email = readString("Email: ");

    contacts.push_back(c);

    std::cout << "Контакт додано.\n";
}

// Показ усіх контактів
void showContacts(const std::vector<Contact>& contacts) {

    std::cout << "\n--- Контакти ---\n";

    if (contacts.empty()) {
        std::cout << "Список порожній.\n";
        return;
    }

    for (size_t i = 0; i < contacts.size(); i++) {
        printContact(i + 1, contacts[i]);
    }
}

// Пошук контакту за ім'ям
void searchContact(const std::vector<Contact>& contacts) {

    std::string name =
        readString("Введіть ім'я для пошуку: ");

    bool found = false;

    std::cout << "\nРезультати:\n";

    for (size_t i = 0; i < contacts.size(); i++) {

        if (contacts[i].name.find(name)
            != std::string::npos) {

            printContact(i + 1, contacts[i]);

            found = true;
        }
    }

    if (!found)
        std::cout << "Контакт не знайдено.\n";
}

// Зчитування номера меню
int readNumber(int min, int max) {

    while (true) {

        std::string line;

        getline(std::cin, line);

        std::stringstream ss(line);

        int number;

        if (ss >> number &&
            ss.eof() &&
            number >= min &&
            number <= max) {

            return number;
        }

        std::cout << "Невірний ввід. Повторіть: ";
    }
}

// Редагування номера телефону
void editPhone(std::vector<Contact>& contacts) {

    if (contacts.empty()) {
        std::cout << "Список порожній.\n";
        return;
    }

    showContacts(contacts);

    std::cout << "\nНомер контакту: ";

    int index =
        readNumber(1,
        static_cast<int>(contacts.size())) - 1;

    contacts[index].phone =
        readString("Новий номер: ");

    std::cout << "Номер змінено.\n";
}

// Видалення контакту
void deleteContact(std::vector<Contact>& contacts) {

    if (contacts.empty()) {
        std::cout << "Список порожній.\n";
        return;
    }

    showContacts(contacts);

    std::cout << "\nНомер контакту: ";

    int index =
        readNumber(1,
        static_cast<int>(contacts.size())) - 1;

    contacts.erase(contacts.begin() + index);

    std::cout << "Контакт видалено.\n";
}

// Виведення меню
void showMenu() {

    std::cout << "\n=== Менеджер контактів ===\n";
    std::cout << "1. Додати контакт\n";
    std::cout << "2. Показати контакти\n";
    std::cout << "3. Пошук за ім'ям\n";
    std::cout << "4. Редагувати номер\n";
    std::cout << "5. Видалити контакт\n";
    std::cout << "0. Зберегти і вийти\n";
}

// Головна функція програми
int main() {

    const std::string filename = "contacts.txt";

    auto contacts = loadContacts(filename);

    while (true) {

        showMenu();

        std::cout << "Ваш вибір: ";

        int choice = readNumber(0, 5);

        switch (choice) {

            case 1:
                addContact(contacts);
                break;

            case 2:
                showContacts(contacts);
                break;

            case 3:
                searchContact(contacts);
                break;

            case 4:
                editPhone(contacts);
                break;

            case 5:
                deleteContact(contacts);
                break;

            case 0:

                saveContacts(filename, contacts);

                std::cout
                    << "Дані збережено.\n";

                return 0;
        }
    }

    return 0;
}