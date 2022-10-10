//#include "Avl_tree.h"
//#include "Node.h"
#include "TreeTypes.h"



#ifndef DSW_H
#define DSW_H

namespace DS {
struct CompareCarGrades;
struct CompareCarTypeSold;
struct CompareCarTypeUnsold;
struct CompareCarModels;

class dsw_company {
   private:
    int num_of_items;
    // both trees have inner trees in there val classes
    Tree<CarTypeSoldValues, CompareCarType> carTypeTreeSold;
    Tree<CarTypeUnsoldValues, CompareCarType> carTypeTreeUnsold;
    Tree<CarType, CompareCarType> carTypesTree;

    // dont have inner trees in val
    Tree<GradeValues, CompareCarGrades> mainGradeTree;
    Tree<SalesValues, CompareCarSales> mainSalesTree;

   public:
    dsw_company(const CompareCarType& cmp_car_type, const CompareCarSales& cmp_car_sales,
                const CompareCarGrades& cmp_grade)
        : num_of_items(0),
          carTypeTreeSold(cmp_car_type),
          carTypeTreeUnsold(cmp_car_type),
          carTypesTree(cmp_car_type),
          mainGradeTree(cmp_grade),
          mainSalesTree(cmp_car_sales){};
    ~dsw_company(){};

    StatusType AddCarType(int typeID, int numOfModels)
    {
        if (typeID <= 0 || numOfModels <= 0)
        {
            return INVALID_INPUT;
        }
        // check if cartype exists
        // carTypeTreeUnsold.compare
        CompareCarSales cmp;
        CompareCarModels cmp_model;
        CarTypeUnsoldValues unsoldVal(typeID, cmp);
        CarTypeSoldValues soldVal(typeID, 0, cmp);
        CarType general_val(typeID, 0, cmp_model);

        /*CarTypeUnsoldValues unsoldVal(typeID, mainSalesTree.Compare);
        CarTypeSoldValues soldVal(typeID, 0, mainSalesTree.Compare);*/
        if (carTypeTreeUnsold.find(unsoldVal) != nullptr || carTypeTreeSold.find(soldVal) != nullptr) {
            return FAILURE;
        }
        // else add cartype node
        num_of_items += numOfModels;
        SalesValues min(typeID, 0);
        SalesValues max(typeID, numOfModels - 1);

        Node<CarTypeUnsoldValues>* newcarTypeUnsold = new Node<CarTypeUnsoldValues>(unsoldVal);
        carTypeTreeUnsold.AddNode(newcarTypeUnsold);

        // initialise not sold tree
        newcarTypeUnsold->val.tree.root = newcarTypeUnsold->val.tree.insertArray(typeID, 0, numOfModels - 1);
        newcarTypeUnsold->val.tree.min = newcarTypeUnsold->val.tree.find(min);
        newcarTypeUnsold->val.tree.max = newcarTypeUnsold->val.tree.find(max);
        newcarTypeUnsold->val.modelCount = numOfModels;

        // initialise general tree
        Node<CarType>* CarTypes = new Node<CarType>(general_val);
        carTypesTree.AddNode(CarTypes);
        CarTypes->val.tree.root = CarTypes->val.tree.insertArray(typeID, 0, numOfModels - 1);
        CarTypes->val.tree.min = CarTypes->val.tree.find(min);
        CarTypes->val.tree.max = CarTypes->val.tree.find(max);
        CarTypes->val.modelCount = numOfModels;

        // add to sold
        Node<CarTypeSoldValues>* newCarTypesold = new Node<CarTypeSoldValues>(soldVal);

        carTypeTreeSold.AddNode(newCarTypesold);
        newCarTypesold->val.modelCount = numOfModels;

        return SUCCESS;
    };

    StatusType RemoveCarType(int typeID)
    {
        // remove from unsold and sold tree
        // mainSalesTree.val mainSalesTree.Compare are passed as default argument
        // only typeID is relevant for the search
        CarTypeUnsoldValues unsoldVal(typeID, mainSalesTree.Compare);
        CarTypeSoldValues soldVal(typeID, 0, mainSalesTree.Compare);
        CarType val(typeID, 0, CompareCarModels());

        // if not found in trees return FAILURE
        Node<CarTypeUnsoldValues>* unsoldCarType = carTypeTreeUnsold.find(unsoldVal);
        Node<CarTypeSoldValues>* soldCarType = carTypeTreeSold.find(soldVal);
        Node<CarType>* generalCarType = carTypesTree.find(val);

        if (soldCarType == nullptr && unsoldCarType == nullptr) {
            return FAILURE;
        }

        // delete all internal nodes from unsold
        while (unsoldCarType != nullptr && unsoldCarType->val.tree.root != nullptr) {
            unsoldCarType->val.tree.Remove(unsoldCarType->val.tree.root->val);
        }

        // delete from main grade and sales tree
        // for this we iterate through all the models in the carType node of carType tree
        // and use their values to search and remove the models in the main trees

        Node<SalesValues>* to_delete;
        if (generalCarType) {
            // get grade of all models from the sold cartype all unsold models wont be in main grade tree
            to_delete = generalCarType->val.tree.min;
            
            

            // build grade value and sales value
            while (to_delete != nullptr) {
                GradeValues grade_val(to_delete->val.model, to_delete->val.carType, to_delete->val.grade);
                mainGradeTree.Remove(grade_val);
                mainSalesTree.Remove(to_delete->val);
                to_delete = generalCarType->val.tree.getNext(to_delete);
            }

            // delete all internal nodes from sold
            while (soldCarType != nullptr  && soldCarType->val.tree.root != nullptr) {
                soldCarType->val.tree.Remove(soldCarType->val.tree.root->val);
            }
            if (soldCarType)
            {
                num_of_items -= soldCarType->val.modelCount;
            }
        }

        // delete all internal nodes from general
        while (generalCarType != nullptr && generalCarType->val.tree.root != nullptr) {
            generalCarType->val.tree.Remove(generalCarType->val.tree.root->val);
        }
        // remove from main carType trees
        StatusType unsoldResult = carTypeTreeUnsold.Remove(unsoldVal);
        StatusType soldResult = carTypeTreeSold.Remove(soldVal);
        StatusType generalResult = carTypesTree.Remove(generalCarType->val);

        if (unsoldResult != SUCCESS && soldResult != SUCCESS && generalResult != SUCCESS) {
            return FAILURE;
        }
        return SUCCESS;
    }

    StatusType SellCar(int typeID, int modelID)
    {
        
        CarType typeVal(typeID, 0, CompareCarModels());
        SalesValues car(typeID, modelID);

        // check if cartype in general
        Node<CarType>* CarType = carTypesTree.find(typeVal);
        // if is not in general tree then wasnt added
        if (CarType == nullptr || CarType->val.modelCount <= modelID) {
            return FAILURE;
        }

        // change grade and sales to model in general tree searches by model
        Node<SalesValues>* model = CarType->val.tree.find(car);
        car.grade = model->val.grade;
        car.sales = model->val.sales;
        // car type node in sold car types
        CarTypeSoldValues SoldValues(typeID, 0, mainSalesTree.Compare);
        Node<CarTypeSoldValues>* soldCarType = carTypeTreeSold.find(SoldValues);

        // if wasnt sold yet
        if (model->val.sales == 0) {
            CarTypeUnsoldValues unsoldTypeValue(typeID, mainSalesTree.Compare);
            Node<CarTypeUnsoldValues>* unsoldCarType = carTypeTreeUnsold.find(unsoldTypeValue);

            // delete model from cartype node in unsoldtree and add to soldtree
            unsoldCarType->val.tree.Remove(car);
            // if is last model in tree than remove cartype from unsold tree
            if (unsoldCarType->val.tree.min == nullptr) {
                carTypeTreeUnsold.Remove(unsoldCarType->val);
            }

            // add to inner sold tree
            SalesValues val_add(typeID, modelID, 10, 1);
            Node<SalesValues>* to_add = new Node<SalesValues>(val_add);
            soldCarType->val.tree.AddNode(to_add);

            // add to grade tree
            GradeValues grade_add(modelID, typeID, 10);
            Node<GradeValues>* to_addgrade = new Node<GradeValues>(grade_add);
            mainGradeTree.AddNode(to_addgrade);

            // add to main sales tree
            Node<SalesValues>* to_add_main = new Node<SalesValues>(val_add);
            mainSalesTree.AddNode(to_add_main);

            // update in general tree
            model->val.sales++;
            model->val.grade += 10;
            return SUCCESS;
        }

        // remove and add new
        GradeValues gradeValue(modelID, typeID, model->val.grade);
        GradeValues new_gradeValue(modelID, typeID, model->val.grade + 10);
        SalesValues new_SalesValue(typeID, modelID, model->val.grade + 10, model->val.sales + 1);

        // remove and add to inner sold tree
        soldCarType->val.tree.Remove(model->val);
        soldCarType->val.tree.AddNode(new Node<SalesValues>(new_SalesValue));

        // change grade tree
        mainGradeTree.Remove(gradeValue);
        mainGradeTree.AddNode(new Node<GradeValues>(new_gradeValue));

        // change sales tree
        mainSalesTree.Remove(model->val);
        mainSalesTree.AddNode(new Node<SalesValues>(new_SalesValue));

        // update in general tree
        model->val.sales++;
        model->val.grade += 10;

        return SUCCESS;
    }

    StatusType MakeComplaint(int typeID, int modelID, int t)
    {
        if (modelID < 0 || t <= 0) return FAILURE;  // INVALID_INPUT
        // build cartype values
        CarTypeUnsoldValues unsoldtype(typeID, mainSalesTree.Compare);
        CarTypeSoldValues SoldValues(typeID, 0, mainSalesTree.Compare);
        CarType generalVal(typeID, 0, CompareCarModels());
        SalesValues car(typeID, modelID);

        // check if model is in trees
        Node<CarTypeSoldValues>* soldCarType = carTypeTreeSold.find(SoldValues);
        Node<CarType>* CarType = carTypesTree.find(generalVal);
        if (CarType == nullptr) {
            return FAILURE;
        }
        Node<SalesValues>* model = CarType->val.tree.find(car);

        // if is not in sold tree then wasnt added or wasnt sold yet
        if (soldCarType == nullptr || soldCarType->val.modelCount <= modelID || model->val.sales == 0) {
            return FAILURE;
        }
        // else change grade in model
        int complaint_grade = 100 / t;
        // build values
        GradeValues grade_car(modelID, typeID, model->val.grade);
        GradeValues new_grade_car(modelID, typeID, model->val.grade - complaint_grade);
        SalesValues sales_value(typeID, modelID, model->val.grade, model->val.sales);

        // change grade in general tree
        model->val.grade -= complaint_grade;

        // change grade tree remove and add new
        mainGradeTree.Remove(grade_car);
        mainGradeTree.AddNode(new Node<GradeValues>(new_grade_car));

        // because in sales tree is compared by sales dont need to remove
        // change sales tree
        Node<SalesValues>* carSales = mainSalesTree.find(sales_value);
        carSales->val.grade -= complaint_grade;

        // change inner sold tree
        soldCarType->val.tree.Remove(sales_value);
        sales_value.grade -= complaint_grade;
        
        soldCarType->val.tree.AddNode(new Node<SalesValues>(sales_value));
        return SUCCESS;
    };

    StatusType GetBestSellerModelByType(int typeID, int* modelID)
    {
        // return best seller from all types
        if (typeID == 0) {
            Node<SalesValues>* mostSales = mainSalesTree.max;
            if (mostSales == nullptr) {
                return FAILURE;
            }
            // check if there is a lower cartype and model with same sales
            Node<SalesValues>* curr = mostSales;
            while (mainSalesTree.getPrev(curr) 
                &&mainSalesTree.getPrev(curr)->val.sales == curr->val.sales) {
                curr = mainSalesTree.getPrev(curr);
            }
            *modelID = curr->val.model;
            return SUCCESS;
        }
        // return best seller from cartype typeID
        else {
            // check if model is in sold tree
            CarTypeSoldValues SoldValues(typeID, 0, mainSalesTree.Compare);
            Node<CarTypeSoldValues>* soldCarType = carTypeTreeSold.find(SoldValues);
            if (!soldCarType || !soldCarType->val.tree.max) {
                //if type was never sold, look in unsold tree
                CarTypeUnsoldValues unSoldValues(typeID, mainSalesTree.Compare);
                Node<CarTypeUnsoldValues>* CarType = carTypeTreeUnsold.find(unSoldValues);
                if (CarType && CarType->val.tree.min)
                {
                    *modelID = CarType->val.tree.min->val.model;
                    return SUCCESS;
                }
                return FAILURE;
            }
            // check if there is a lower cartype and model with same sales
            Node<SalesValues>* curr = soldCarType->val.tree.max;
            while (mainSalesTree.getPrev(curr)
                && mainSalesTree.getPrev(curr)->val.sales == curr->val.sales) {
                curr = mainSalesTree.getPrev(curr);
            }
            *modelID = curr->val.model;
        }
        return SUCCESS;
    };
    /*  begin in main grade tree
            as long as grades are lower an 0 return the models and types inorder
            starting from min
            when encounters node with grade 0 criss cross between unsold tree cartypes and main grade treet
    */
    StatusType GetWorstModels(int numOfModels, int* types, int* models)
    {
        
        if (num_of_items < numOfModels) {
            return FAILURE;
        }

        Node<GradeValues>* mainTreeModel = mainGradeTree.min;

        // get first cartype in unsold tree
        Node<CarTypeUnsoldValues>* carTypeUnsold = carTypeTreeUnsold.min;
        // if carTypeUnsold is null from the begining then only search main grade tree
        if (carTypeUnsold) {
            // get first model in inner tree of first car type
            Node<SalesValues>* unsoldCarModel = carTypeUnsold->val.tree.min;
            GradeValues unsoldCarGrade(unsoldCarModel->val.model, unsoldCarModel->val.carType,
                                       unsoldCarModel->val.grade);
            for (int i = 0; i < numOfModels; i++) {
                // finished unsold car types or unsoldCarModel > mainTreeModel
                if (unsoldCarModel == nullptr || (mainTreeModel != nullptr &&
                    mainGradeTree.Compare(unsoldCarGrade, mainTreeModel->val) > 0)) {
                    // unsoldCarModel > MainTreeModel
                    types[i] = mainTreeModel->val.carType;
                    models[i] = mainTreeModel->val.model;
                    mainTreeModel = mainGradeTree.getNext(mainTreeModel);
                }
                // finished main grade models tree or unsoldCarModel < mainTreeModel
                else if (mainTreeModel == nullptr ||( unsoldCarModel != nullptr &&
                         mainGradeTree.Compare(unsoldCarGrade, mainTreeModel->val) < 0)){
                    // unsoldCarModel < mainTreeModel
                    types[i] = unsoldCarGrade.carType;
                    models[i] = unsoldCarGrade.model;

                    unsoldCarModel = carTypeUnsold->val.tree.getNext(unsoldCarModel);
                    // finished cartype
                    if (!unsoldCarModel) {
                        carTypeUnsold = carTypeTreeUnsold.getNext(carTypeUnsold);
                        // if has more cartype in unsold tree
                        if (carTypeUnsold) {
                            unsoldCarModel = carTypeUnsold->val.tree.min;
                        }
                    }
                    if (unsoldCarModel) {
                        unsoldCarGrade.model = unsoldCarModel->val.model;
                        unsoldCarGrade.carType = unsoldCarModel->val.carType;
                        unsoldCarGrade.grade = unsoldCarModel->val.grade;
                    }
                    
                }
            }
        }
        else {
            for (int i = 0; i < numOfModels; i++) {
                types[i] = mainTreeModel->val.carType;
                models[i] = mainTreeModel->val.model;
                mainTreeModel = mainGradeTree.getNext(mainTreeModel);
            }
        }
        return SUCCESS;
    };

    void Quit()
    {
        while (carTypesTree.root != nullptr) {
            RemoveCarType(carTypesTree.root->val.carType);
        }
    };
    

};
}  // namespace DS

#endif