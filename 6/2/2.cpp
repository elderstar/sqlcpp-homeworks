#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <vector>
#include <set>
#include <Windows.h>

class Book;
class Stock;
class Sale;

namespace dbo = Wt::Dbo;

typedef dbo::collection<dbo::ptr<Book>> Books;
typedef dbo::collection<dbo::ptr<Stock>> Stocks;
typedef dbo::collection<dbo::ptr<Sale>> Sales;

class Publisher
{
public:
    std::string name = "";
    Books books;

    template<typename Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book
{
public:
    std::string title = "";
    dbo::ptr<Publisher> publisher;
    Stocks stocks;

    template<typename Action>
    void persist(Action& a)
    {
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Shop
{
public:
    std::string name = "";
    Stocks stocks;

    template<typename Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");
    }
};

class Stock
{
public:
    int count = 0;
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;
    Sales sales;

    template<typename Action>
    void persist(Action& a)
    {
        dbo::field(a, count, "count");
        dbo::belongsTo(a, book, "book");
        dbo::belongsTo(a, shop, "shop");
        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");
    }
};

class Sale
{
public:
    int count = 0;
    int price = 0;
    std::string date_sale = "";
    dbo::ptr<Stock> stock;

    template<typename Action>
    void persist(Action& a)
    {
        dbo::field(a, count, "count");
        dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::belongsTo(a, stock, "stock");
    }
};

class DB
{
public:
    DB(std::string host_, int port_, std::string dbname_, std::string user_, std::string password_)
    {
        connection = std::make_unique<dbo::backend::Postgres>("host=" + host_ +
            " port=" + std::to_string(port_) +
            " dbname=" + dbname_ +
            " user=" + user_ +
            " password=" + password_);
        
        session.setConnection(std::move(connection));
        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Stock>("stock");
        session.mapClass<Shop>("shop");
        session.mapClass<Sale>("sale");

        try
        {
            session.createTables();
            initTables();
        }
        catch (const dbo::Exception& e)
        {
            std::cout << e.what();
        }
    }
    ~DB()
    {
        connection.release();
    }
    void initTables()
    {
        dbo::ptr<Shop> s0 = addShop("Прометей");
        dbo::ptr<Shop> s1 = addShop("Хорошие Книги");
        dbo::ptr<Shop> s2 = addShop("Дом Книги");
        addPublisher("тест");

        dbo::ptr<Publisher> p = addPublisher("Махаон");
        dbo::ptr<Book> b = addBook("Гарри Поттер и философский камень", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и Тайная комната", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и узник Азкабана", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и Кубок Огня", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и Орден Феникса", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и Принц - Полукровка", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);
        b = addBook("Гарри Поттер и ПринцДары Смерти", p);
        addStock(b, s0, 10);
        addStock(b, s1, 10);

        p = addPublisher("AB Publishing");
        b = addBook("Подсознание может всё!", p);
        addStock(b, s1, 10);
        b = addBook("Богатый папа, бедный папа", p);
        addStock(b, s1, 10);
        b = addBook("Как завоевывать друзей и оказывать влияние на людей", p);
        addStock(b, s1, 5);

        p = addPublisher("Манн, Иванов и Фербер");
        b = addBook("Эмоциональный интеллект ребенка. Практическое руководство для родителей", p);
        addStock(b, s2, 15);
        addStock(b, s1, 10);
        b = addBook("Будь лучшей версией себя. Как обычные люди становятся выдающимися", p);
        addStock(b, s2, 5);
        addStock(b, s1, 5);
        b = addBook("Выйди из зоны комфорта. Измени свою жизнь.", p);
        addStock(b, s2, 5);
        addStock(b, s1, 10);
    }

    dbo::ptr<Book> addBook(const std::string& title, const dbo::ptr<Publisher>& publisher = nullptr)
    {
        dbo::Transaction transaction(session);
        std::unique_ptr<Book> new_book {new Book()};
        new_book->title = title;

        if (publisher)
        {
            new_book->publisher = publisher;
        }
        dbo::ptr<Book> b = session.add(std::move(new_book));
        transaction.commit();
        return b;
    }

    dbo::ptr<Shop> addShop(const std::string& name)
    {
        dbo::Transaction transaction(session);
        std::unique_ptr<Shop> new_shop {new Shop()};
        new_shop->name = name;

        dbo::ptr<Shop> p = session.add(std::move(new_shop));
        transaction.commit();
        return p;
    }

    dbo::ptr<Publisher> addPublisher(const std::string& name)
    {
        dbo::Transaction transaction(session);
        std::unique_ptr<Publisher> new_publisher {new Publisher()};
        new_publisher->name = name;

        dbo::ptr<Publisher> p = session.add(std::move(new_publisher));
        transaction.commit();
        return p;
    }

    Books getBooks(const dbo::ptr<Publisher>& publisher)
    {
        try
        {
            dbo::Transaction transaction(session);
            Books books = publisher->books;
            transaction.commit();
            return books;
        }
        catch (const dbo::Exception& e)
        {
            std::cout << e.what();
        }
    }

    dbo::ptr<Publisher> getPublisher(const std::string& name)
    {
        try
        {
            dbo::Transaction transaction(session);
            dbo::ptr<Publisher> p = session.find<Publisher>().where("name = ?").bind(name);
            transaction.commit();
            return p;
        }
        catch (const dbo::Exception& e)
        {
            std::cout << e.what();
        }
    }

    void syncPublisherToBook(const dbo::ptr<Publisher>& p, const dbo::ptr<Book>& b)
    {
        dbo::Transaction transaction(session);
        b.modify()->publisher = p;
        transaction.commit();
    }

    void addStock(const dbo::ptr<Book>& b, const dbo::ptr<Shop>& s, const int & count)
    {
        dbo::Transaction transaction(session);
        std::unique_ptr<Stock> new_stock {new Stock()};
        new_stock->book = b;
        new_stock->shop = s;
        new_stock->count = count;

        dbo::ptr<Stock> stock = session.add(std::move(new_stock));
        transaction.commit();
    }

private:
    

    dbo::Session session;
    std::unique_ptr<dbo::backend::Postgres> connection;
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif // _WIN32
}
#ifdef WIN32
std::string cp1251_to_utf8(const std::string& cp1251) {
    std::string res;
    int result_u, result_c;
    enum { CP1251 = 1251 };
    result_u = MultiByteToWideChar(CP1251, 0, cp1251.c_str(), -1, 0, 0);
    if (!result_u) {
        throw std::runtime_error("cp1251_to_utf8 cannot convert MultiByteToWideChar!");
    }
    wchar_t* ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(CP1251, 0, cp1251.c_str(), -1, ures, result_u)) {
        delete[] ures;
        throw std::runtime_error("cp1251_to_utf8 cannot convert MultiByteToWideChar 2!");
    }
    result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c) {
        delete[] ures;
        throw std::runtime_error("cp1251_to_utf8 cannot convert WideCharToMultiByte!");
    }
    char* cres = new char[result_c];
    if (!WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0)) {
        delete[] cres;
        throw std::runtime_error("cp1251_to_utf8 cannot convert WideCharToMultiByte 2!");
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

#endif

std::string readInput()
{
    std::string input;
    std::getline(std::cin, input);
#ifdef WIN32
    input = cp1251_to_utf8(input);
#endif
    return input;
}

int main()
{
    system("chcp 1251");
    setlocale(LC_ALL, "ru_RU.UTF-8");

    try
    {
        DB db = DB("localhost", 5432, "lesson6_db", "postgres", "postgres");
        std::string input;
        clearScreen();
        dbo::ptr<Publisher> pub = {};
        while (true) 
        {
            std::cout << "Введите имя издателя: ";
            std::getline(std::cin, input);
            input = cp1251_to_utf8(input);
            if (!input.empty())
            {
                pub = db.getPublisher(input);
                if(pub)
                    break;
                else
                    std::cout << "Данный издатель не найден\n";
            }
        }
        clearScreen();
        if (pub) 
        {
            Books pub_books = db.getBooks(pub);

            dbo::Transaction t(*pub.session());
            std::set<dbo::ptr<Shop>> shops = {};

            if (pub_books.size() > 0) 
            {
                for (const auto& book : pub_books)
                {
                    for (const auto& stock : book->stocks)
                    {
                        shops.insert(stock->shop);
                    }
                }
                if (shops.size() > 0)
                {
                    std::cout << "Книги данного издателя есть в магазинах:\n";
                    for (const auto& shop : shops)
                    {
                        std::cout << shop.id() << ") " << shop->name << "\n";
                    }
                }
            }
            else {
                std::cout << "Книги данного издателя не найдены";
            }
        }
    }
    catch (const dbo::Exception& e)
    {
        std::cout << e.what();
    }
}