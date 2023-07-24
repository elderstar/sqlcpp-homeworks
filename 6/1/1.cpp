#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

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

int main()
{
    try
    {
        auto connection = std::make_unique<dbo::backend::Postgres>("host=localhost "
            "port=5432 "
            "dbname=lesson6_db "
            "user=postgres "
            "password=postgres");
        dbo::Session session;
        session.setConnection(std::move(connection));
        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Stock>("stock");
        session.mapClass<Shop>("shop");
        session.mapClass<Sale>("sale");

        try
        {
            session.createTables();
        }
        catch (const dbo::Exception& e)
        {
            std::cout << e.what();
        }
    }
    catch (const dbo::Exception& e)
    {
        std::cout << e.what();
    }
}