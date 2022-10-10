#include "library.h" 
#include "dsw.h"

void *Init(){
    DS::CompareCarGrades cmp_grade_val;
    DS::CompareCarType cmp_car;
    DS::CompareCarSales cmp_sales;
    DS::dsw_company *DS = new DS::dsw_company(cmp_car, cmp_sales, cmp_grade_val);
    return (void *)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels){
    return ((DS::dsw_company *)DS)->AddCarType(typeID, numOfModels);
}

StatusType RemoveCarType(void *DS, int typeID) {
    return ((DS::dsw_company *)DS)->RemoveCarType(typeID);
}

StatusType SellCar(void *DS, int typeID, int modelID){ 
    return ((DS::dsw_company *)DS)->SellCar(typeID, modelID);
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t){
    return ((DS::dsw_company *)DS)->MakeComplaint(typeID, modelID, t);
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int *modelID){
    return ((DS::dsw_company *)DS)->GetBestSellerModelByType(typeID, modelID);
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models){
    return ((DS::dsw_company *)DS)->GetWorstModels(numOfModels, types, models);
}

void Quit(void **DS){
    if ((DS::dsw_company *)DS == nullptr) {
        return;
    }
    ((DS::dsw_company *)DS)->Quit();
    DS::dsw_company * to_delete = static_cast<DS::dsw_company *>(*DS);
    delete to_delete;
    *DS = nullptr;
    return;
}
