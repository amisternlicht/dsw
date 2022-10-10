#include "Avl_tree.h"
#ifndef TREETYPE_H
#define TREETYPE_H
namespace DS {

    /* ____________________________________ initializers ____________________________________*/
    class CarType;
    class GradeValues;
    class SalesValues;
    struct CompareCarSales;
    class CarTypeSoldValues;
    struct CompareCarGrades;
    struct CompareCarTypeSold;
    struct CompareCarTypeUnsold;
    struct CompareCarModels;

    /* ____________________________________ Nodes ____________________________________*/
    /*
        Node in cartype-general tree
        additionaly each node has an inner tree which compares by model
    */
    class CarType {
    public:
        int carType;
        // count of all models, sold & unsold
        int modelCount;
        // inner tree compare by model
        Tree<SalesValues, CompareCarModels> tree;
        CarType(int _carType, int _model_count, const CompareCarModels& cmp_sales)
            : carType(_carType), modelCount(_model_count), tree(cmp_sales) {};
        ~CarType() {};
    };
    /*
        Node in cartype-sold tree
        additionaly each node has an inner tree which compares by sales (only models will count)
    */
    class CarTypeSoldValues {
    public:
        int carType;
        // count of all models, sold & unsold
        int modelCount;
        // inner tree compare by sales
        Tree<SalesValues, CompareCarSales> tree;
        CarTypeSoldValues(int _carType, int _model_count, const CompareCarSales& cmp_sales)
            : carType(_carType), modelCount(_model_count), tree(cmp_sales) {};
        ~CarTypeSoldValues() {};
    };
    /*
        Node in cartype-unsold tree
        additionaly each node has an inner tree which compares by sales(only models will count)
    */
    class CarTypeUnsoldValues {
    public:
        int carType;
        // count of all models, sold & unsold
        int modelCount;
        // inner tree compare by sales
        Tree<SalesValues, CompareCarSales> tree;
        CarTypeUnsoldValues(int _carType, const CompareCarSales& cmp_sales)
            : carType(_carType), modelCount(0), tree(cmp_sales) {};
        ~CarTypeUnsoldValues() {};
    };
    /*
        Node in main grades tree
    */
    class GradeValues {
    public:
        int model;
        int carType;
        int grade;
        GradeValues(int _model, int _carType) : model(_model), carType(_carType), grade(0) {};
        GradeValues(int _model, int _carType, int _grade) : model(_model), carType(_carType), grade(_grade) {};
        ~GradeValues() {};
    };
    /*
        Node in main sales tree
        and in inner trees of sold and unsold cartypetrees
    */
    class SalesValues {
    public:
        int carType;
        int model;
        int grade;
        int sales;
        SalesValues(int _carType, int _model = 0, int _grade = 0, int _sales = 0)
            : carType(_carType), model(_model), grade(_grade), sales(_sales) {};
        SalesValues(SalesValues& salesVal)
            : carType(salesVal.carType), model(salesVal.model), grade(salesVal.grade), sales(salesVal.sales) {};
        ~SalesValues() {};
    };
    
    /* ____________________________________ comparators ____________________________________*/
    // compare by grade
    struct CompareCarGrades {
    public:
        CompareCarGrades() = default;
        ~CompareCarGrades() = default;
        int operator()(const GradeValues& a, const GradeValues& b) const
        {
            int res = a.grade - b.grade;
            if (res != 0) return res;
            res = a.carType - b.carType;
            if (res != 0) return res;
            res = a.model - b.model;
            return res;
        }
    };
    // compare by sales
    struct CompareCarSales {
    public:
        CompareCarSales() = default;
        CompareCarSales(const CompareCarSales& c) {};
        ~CompareCarSales() = default;
        int operator()(const SalesValues& a, const SalesValues& b) const
        {
            int res = a.sales - b.sales;
            if (res != 0) return res;
            res = a.carType - b.carType;
            if (res != 0) return res;
            res = a.model - b.model;
            return res;
        }
    };
    // compare by model
    struct CompareCarModels {
    public:
        CompareCarModels() = default;
        CompareCarModels(const CompareCarModels& c) {};
        ~CompareCarModels() = default;
        int operator()(const SalesValues& a, const SalesValues& b) const
        {
            int res = a.carType - b.carType;
            if (res != 0) return res;
            res = a.model - b.model;
            return res;
        }
    };
    // compare by cartype
    struct CompareCarType {
    public:
        CompareCarType(const CompareCarType& c) {};
        CompareCarType() = default;
        ~CompareCarType() = default;
        int operator()(const CarTypeUnsoldValues& a, const CarTypeUnsoldValues& b) const { return a.carType - b.carType; }
        int operator()(const CarTypeSoldValues& a, const CarTypeSoldValues& b) const { return a.carType - b.carType; }
        int operator()(const CarType& a, const CarType& b) const { return a.carType - b.carType; }
    };
}  // namespace DS

#endif