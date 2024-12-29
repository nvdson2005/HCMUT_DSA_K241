#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

#include <filesystem> //require C++17
namespace fs = std::filesystem;

#include "list/listheader.h"
#include "sformat/fmt_lib.h"
#include "tensor/xtensor_lib.h"
#include "ann/annheader.h"
#include "loader/dataset.h"
#include "loader/dataloader.h"
#include "config/Config.h"
#include "dataset/DSFactory.h"
#include "optim/Adagrad.h"
#include "optim/Adam.h"
#include "modelzoo/twoclasses.h"
#include "modelzoo/threeclasses.h"

//Additional include
#include "util/Point.h"
#include "util/ArrayLib.h"
#include "util/sampleFunc.h"
#include "heap/Heap.h"
void mlpDemo1() {
    xt::random::seed(42);
    DSFactory factory("./config.txt");
    xmap<string, TensorDataset<double, double>*>* pMap = factory.get_datasets_2cc();
    TensorDataset<double, double>* train_ds = pMap->get("train_ds");
    TensorDataset<double, double>* valid_ds = pMap->get("valid_ds");
    TensorDataset<double, double>* test_ds = pMap->get("test_ds");
    DataLoader<double, double> train_loader(train_ds, 50, true, false);
    DataLoader<double, double> valid_loader(valid_ds, 50, false, false);
    DataLoader<double, double> test_loader(test_ds, 50, false, false);

    cout << "Train dataset: " << train_ds->len() << endl;
    cout << "Valid dataset: " << valid_ds->len() << endl;
    cout << "Test dataset: " << test_ds->len() << endl;

    int nClasses = 2;
    ILayer* layers[] = {
        new FCLayer(2, 50, true),
        new ReLU(),
        new FCLayer(50, nClasses, true),
        new Softmax()
    };

    MLPClassifier model("./config.txt", "2c-classification", layers, sizeof(layers)/sizeof(ILayer*));

    SGD optim(2e-3);
    CrossEntropy loss;
    ClassMetrics metrics(nClasses);

    model.compile(&optim, &loss, &metrics);
    model.fit(&train_loader, &valid_loader, 10);
    string base_path = "./models";
    // model.save(base_path + "/" + "2c-classification-1");
    double_tensor eval_rs = model.evaluate(&test_loader);
    cout << "Evaluation result on the testing dataset: " << endl;
    cout << eval_rs << endl;
}
void mlpDemo2() {
    xt::random::seed(42);
    DSFactory factory("./config.txt");
    xmap<string, TensorDataset<double, double>*>* pMap = factory.get_datasets_2cc();
    TensorDataset<double, double>* train_ds = pMap->get("train_ds");
    TensorDataset<double, double>* valid_ds = pMap->get("valid_ds");
    TensorDataset<double, double>* test_ds = pMap->get("test_ds");
    DataLoader<double, double> train_loader(train_ds, 50, true, false);
    DataLoader<double, double> valid_loader(valid_ds, 50, false, false);
    DataLoader<double, double> test_loader(test_ds, 50, false, false);

    int nClasses = 2;
    ILayer* layers[] = {
        new FCLayer(2, 50, true),
        new Sigmoid(),
        new FCLayer(50, nClasses, true),
        new Softmax()
    };

    MLPClassifier model("./config.txt", "2c-classification", layers, sizeof(layers)/sizeof(ILayer*));

    SGD optim(2e-3);
    CrossEntropy loss;
    ClassMetrics metrics(nClasses);

    model.compile(&optim, &loss, &metrics);
    model.fit(&train_loader, &valid_loader, 1000);
    string base_path = "./models";
    // model.save(base_path + "/" + "2c-classification-1");
    double_tensor eval_rs = model.evaluate(&test_loader);
    cout << "Evaluation result on the testing dataset: " << endl;
    cout << eval_rs << endl;
}
void mlpDemo3() {
    xt::random::seed(42);
    DSFactory factory("./config.txt");
    xmap<string, TensorDataset<double, double>*>* pMap = factory.get_datasets_2cc();
    TensorDataset<double, double>* train_ds = pMap->get("train_ds");
    TensorDataset<double, double>* valid_ds = pMap->get("valid_ds");
    TensorDataset<double, double>* test_ds = pMap->get("test_ds");
    DataLoader<double, double> train_loader(train_ds, 50, true, false);
    DataLoader<double, double> valid_loader(valid_ds, 50, false, false);
    DataLoader<double, double> test_loader(test_ds, 50, false, false);

    int nClasses = 2;
    ILayer* layers[] = {
        new FCLayer(2, 50, true),
        new Tanh(),
        new FCLayer(50, nClasses, true),
        new Softmax()
    };

    MLPClassifier model("./config.txt", "2c-classification", layers, sizeof(layers)/sizeof(ILayer*));

    SGD optim(2e-3);
    CrossEntropy loss;
    ClassMetrics metrics(nClasses);

    model.compile(&optim, &loss, &metrics);
    model.fit(&train_loader, &valid_loader, 10);
    string base_path = "./models";
    // model.save(base_path + "/" + "2c-classification-1");
    double_tensor eval_rs = model.evaluate(&test_loader);
    cout << "Evaluation result on the testing dataset: " << endl;
    cout << eval_rs << endl;
}
/* ******* BEGIN Heap ******* */
void heapDemo1(){
    int array[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    //min heap: [6, 10, 7, 23, 15, 20, 8, 50]
    //max heap: [50, 23, 15, 20, 8, 6, 7, 10]
    Heap<int> minHeap1;
    cout << "Min Heap: ";
    minHeap1.heapify(array, 8);
    cout << minHeap1.toString() << endl;
    
    Heap<int> minHeap2(minHeapComparator);
    cout << "Min Heap: ";
    minHeap2.heapify(array, 8);
    cout << minHeap2.toString() << endl;
    
    Heap<int> maxHeap(maxHeapComparator);
    cout << "Max Heap: ";
    maxHeap.heapify(array, 8);
    cout << maxHeap.toString() << endl;
}

void heapDemo2(){
    Point array[] = {
        Point(13.1, 12.4), Point(5.5, 4.5), Point(15.5, 14.5),
        Point(23.1, 12.4), Point(35.5, 14.5), Point(5.5, 34.5)
    };
    Heap<Point> minHeap1(minHeapComparator);
    cout << "Min Heap: ";
    minHeap1.heapify(array, 6);
    cout << minHeap1.toString(&point2str) << endl;
    
    Heap<Point> maxHeap(maxHeapComparator);
    cout << "Max Heap: ";
    maxHeap.heapify(array, 6);
    cout << maxHeap.toString(&point2str) << endl;
}

int myPointComparator(Point*&  lhs, Point*& rhs){
    if(lhs->radius() < rhs->radius()) return -1;
    else if(lhs->radius() > rhs->radius()) return +1;
    else return 0;
}
int myPointComparatorMAX(Point*&  lhs, Point*& rhs){
    if(lhs->radius() < rhs->radius()) return +1;
    else if(lhs->radius() > rhs->radius()) return -1;
    else return 0;
}

string myPoint2Str(Point*& point){
    stringstream os;
    os << "["   << setw(6) << setprecision(2) << point->getX() 
                << ", "
                << setw(6) << setprecision(2) << point->getY()
       << "]";
    return os.str();
}
void heapDemo3(){
    Point* array[] = {
        new Point(13.1, 12.4), new Point(5.5, 4.5), new Point(15.5, 14.5),
        new Point(23.1, 12.4), new Point(35.5, 14.5), new Point(5.5, 34.5)
    };
    Heap<Point*> heap(&myPointComparatorMAX, &Heap<Point*>::free);
    for(int idx=0; idx < 6; idx++) heap.push(array[idx]);
    heap.println(&myPoint2Str);
    
    for(Heap<Point*>::Iterator it = heap.begin(); it != heap.end(); it++){
        Point* point = *it;
        cout << point->radius() << ", ";
    }
    cout << endl;
}
/* ******* END Heap ******* */

int main(int argc, char** argv) {
    //dataloader:
    //case_data_wo_label_1();
    //case_data_wi_label_1();
    //case_batch_larger_nsamples();
    // test_backward_batch();
    // test_softmax_backward();
    //Classification:
    //twoclasses_classification();
    twoclasses_classification();
    //threeclasses_classification();
    return 0;
}
