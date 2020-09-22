#include "PopulateDatabase.h"

#include <QSqlQuery>
#include <QSqlError>

#ifdef QT_DEBUG
namespace PopulateDatabase_NS {
static const char* CREATE_COLORS_QUERY = "CREATE TABLE public.colors "
                                         "(id serial NOT NULL,"
                                         "color varchar(50) NOT NULL UNIQUE,"
                                         "PRIMARY KEY (id));";

static const char* CREATE_MATERIALS_QUERY = "CREATE TABLE public.materials "
                                            "(id serial NOT NULL, "
                                            "material varchar(50) NOT NULL UNIQUE, "
                                            "PRIMARY KEY (id));";

static const char* CREATE_MODELS_QUERY = "CREATE TABLE public.models"
                                         "(id serial NOT NULL, "
                                         "model varchar(50) NOT NULL UNIQUE, "
                                         "PRIMARY KEY (id));";

static const char* CREATE_ITEMS_QUERY = "CREATE TABLE public.items"
                                        "(id serial NOT NULL, "
                                        "doc_id numeric NOT NULL, "
                                        "shoe_id integer NOT NULL, "
                                        "size integer NOT NULL, "
                                        "entry_price numeric NOT NULL, "
                                        "rebate numeric NOT NULL, "
                                        "rebate_percentage numeric NOT NULL, "
                                        "tax numeric NOT NULL, "
                                        "tax_percentage numeric NOT NULL, "
                                        "sale_price numeric NOT NULL, "
                                        "discount numeric NOT NULL, "
                                        "price_difference numeric NOT NULL, "
                                        "quantity numeric NOT NULL, "
                                        "date timestamp NOT NULL DEFAULT NOW(), "
                                        "updated timestamp NOT NULL DEFAULT NOW(), "
                                        "PRIMARY KEY (id));";

static const char* CREATE_DOCUMENTS_QUERY = "CREATE TABLE public.documents"
                                            "(id serial NOT NULL, "
                                            "doc_type_id numeric NOT NULL, "
                                            "doc_number numeric NOT NULL, "
                                            "entry_price numeric NOT NULL, "
                                            "rebate numeric NOT NULL, "
                                            "rebate_percentage numeric NOT NULL, "
                                            "tax numeric NOT NULL, "
                                            "tax_percentage numeric NOT NULL, "
                                            "sale_price numeric NOT NULL, "
                                            "price_difference numeric NOT NULL, "
                                            "date timestamp NOT NULL DEFAULT NOW(), "
                                            "updated timestamp NOT NULL DEFAULT NOW(), "
                                            "PRIMARY KEY (id));";

static const char* CREATE_DOCUMENT_TYPES_QUERY = "CREATE TABLE public.doc_types"
                                                 "(id serial NOT NULL, "
                                                 "doc_name varchar(50) NOT NULL, "
                                                 "affects boolean NOT NULL, "
                                                 "PRIMARY KEY (id));";

//code integer, can be null (maybe varchar is needed for code?)
static const char* CREATE_SHOES_QUERY = "CREATE TABLE public.shoes("
                                        "id serial NOT NULL, "
                                        "code integer, "
                                        "color integer NOT NULL, "
                                        "model integer NOT NULL, "
                                        "material integer NOT NULL, "
                                        "price numeric NOT NULL, "
                                        "PRIMARY KEY (id), "
                                        "CONSTRAINT color FOREIGN KEY (color) "
                                        "REFERENCES colors (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID, "
                                        "CONSTRAINT material FOREIGN KEY (material) "
                                        "REFERENCES materials (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID, "
                                        "CONSTRAINT model FOREIGN KEY (model) "
                                        "REFERENCES models (id) MATCH SIMPLE "
                                        "ON UPDATE NO ACTION "
                                        "ON DELETE RESTRICT "
                                        "NOT VALID);";

static const char* INSERT_COLORS = "INSERT INTO colors (color) VALUES ('red');"
                                   "INSERT INTO colors (color) VALUES ('blue');"
                                   "INSERT INTO colors (color) VALUES ('green');"
                                   "INSERT INTO colors (color) VALUES ('gray');";

static const char* INSERT_MATERIALS = "INSERT INTO materials (material) VALUES ('leather');"
                                      "INSERT INTO materials (material) VALUES ('artificial leather');"
                                      "INSERT INTO materials (material) VALUES ('rubber');"
                                      "INSERT INTO materials (material) VALUES ('textiles');";

static const char* INSERT_MODELS = "INSERT INTO models (model) VALUES ('ballet');"
                                   "INSERT INTO models (model) VALUES ('boat');"
                                   "INSERT INTO models (model) VALUES ('espadrille');"
                                   "INSERT INTO models (model) VALUES ('derby');";

static const char* INSERT_SHOES = "INSERT INTO shoes (code, color, model, material, size, price) VALUES (123, 1, 1, 1, 40, 1500);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (234, 1, 2, 3, 41, 1200);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (345, 2, 1, 2, 42, 1300);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (456, 3, 2, 1, 39, 1900);"
                                  "INSERT INTO shoes (code, color, model, material, size, price) VALUES (567, 1, 3, 3, 43, 2000);";

static const char* INSERT_DOC_TYPES = "INSERT INTO doc_types (doc_name, affects) VALUES ('receipt invoice', true);"
                                      "INSERT INTO doc_types (doc_name, affects) VALUES ('delivery invoice', false);"
                                      "INSERT INTO doc_types (doc_name, affects) VALUES ('bill', false);";
}

using namespace PopulateDatabase_NS;

PopulateDatabase::PopulateDatabase()
{

}

/*-----------------------------------------------------_PUBLIC FUNCTIONS_-------------------------------------------*/
void PopulateDatabase::createDatabase()
{
    createColors();
    createMaterials();
    createModels();
    createShoes();
    createItems();
    createDocTypes();
    createDocuments();
}

void PopulateDatabase::fillDatabase()
{
    fillColors();
    fillModels();
    fillMaterials();
    fillShoes();
    fillDocTypes();
}
/*-----------------------------------------------------_PUBLIC FUNCTIONS_-------------------------------------------*/

/*-----------------------------------------------------_CREATE DATABASE_-------------------------------------------*/
void PopulateDatabase::createColors()
{
    QSqlQuery q;
    if(!q.exec(CREATE_COLORS_QUERY))
    {
        qWarning()<<"Unable to create table colors, error: "<<q.lastError()<<" last query: "<<q.lastQuery();
    }
}

void PopulateDatabase::createShoes()
{
    QSqlQuery q;
    if(!q.exec(CREATE_SHOES_QUERY))
    {
        qWarning()<<"Unable to create table shoes, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createMaterials()
{
    QSqlQuery q;
    if(!q.exec(CREATE_MATERIALS_QUERY))
    {
        qWarning()<<"Unable to create table materials, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createModels()
{
    QSqlQuery q;
    if(!q.exec(CREATE_MODELS_QUERY))
    {
        qWarning()<<"Unable to create table models, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createItems()
{
    QSqlQuery q;
    if(!q.exec(CREATE_ITEMS_QUERY))
    {
        qWarning()<<"Unable to create table items, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createDocuments()
{
    QSqlQuery q;
    if(!q.exec(CREATE_DOCUMENTS_QUERY))
    {
        qWarning()<<"Unable to create table documents, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::createDocTypes()
{
    QSqlQuery q;
    if(!q.exec(CREATE_DOCUMENT_TYPES_QUERY))
    {
        qWarning()<<"Unable to create table doctypes, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}
/*-----------------------------------------------------_CREATE DATABASE_-------------------------------------------*/


/*-----------------------------------------------------_FILL DATABASE_-------------------------------------------*/
void PopulateDatabase::fillColors()
{
    QSqlQuery q;
    if(!q.exec(INSERT_COLORS))
    {
        qWarning()<<"Unable to insert into colors, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillModels()
{
    QSqlQuery q;
    if(!q.exec(INSERT_MODELS))
    {
        qWarning()<<"Unable to insert into models, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillMaterials()
{
    QSqlQuery q;
    if(!q.exec(INSERT_MATERIALS))
    {
        qWarning()<<"Unable to insert into materials, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillShoes()
{
    QSqlQuery q;
    if(!q.exec(INSERT_SHOES))
    {
        qWarning()<<"Unable to insert into shoes, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}

void PopulateDatabase::fillDocTypes()
{
    QSqlQuery q;
    if(!q.exec(INSERT_DOC_TYPES))
    {
        qWarning()<<"Unable to insert into doc_types, error: "<<q.lastError()<<" lastquery: "<<q.lastQuery();
    }
}
/*-----------------------------------------------------_FILL DATABASE_-------------------------------------------*/
#endif
