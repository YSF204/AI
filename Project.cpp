#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class GenderClassifier {
private:
    double male_heights[4] = {6.00, 5.92, 5.58, 5.92};
    double male_weights[4] = {180, 190, 170, 165};
    double male_foot_sizes[4] = {12, 11, 12, 10};
    
    double female_heights[4] = {5.00, 5.50, 5.42, 5.75};
    double female_weights[4] = {100, 150, 130, 150};
    double female_foot_sizes[4] = {6, 8, 7, 9};
    
    double male_height_mean, male_height_std;
    double male_weight_mean, male_weight_std;
    double male_foot_mean, male_foot_std;
    
    double female_height_mean, female_height_std;
    double female_weight_mean, female_weight_std;
    double female_foot_mean, female_foot_std;

public:
    const double PI = 3.14159 ;

    double Mean(double* arr, int size) {
        double sum = 0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum / size;
    }
    
    // Calculate standard deviation
    double StandardDev(double* arr, int size, double mean) {
        double variance = 0;
        for (int i = 0; i < size; i++) {
            variance += pow(arr[i] - mean, 2);
        }
        return sqrt(variance / size);
    }
    
    void trainModel() {

        male_height_mean = Mean(male_heights, 4);
        male_height_std = StandardDev(male_heights, 4, male_height_mean);
        
        male_weight_mean = Mean(male_weights, 4);
        male_weight_std = StandardDev(male_weights, 4, male_weight_mean);
        
        male_foot_mean = Mean(male_foot_sizes, 4);
        male_foot_std = StandardDev(male_foot_sizes, 4, male_foot_mean);
        
        female_height_mean = Mean(female_heights, 4);
        female_height_std = StandardDev(female_heights, 4, female_height_mean);
        
        female_weight_mean = Mean(female_weights, 4);
        female_weight_std = StandardDev(female_weights, 4, female_weight_mean);
        
        female_foot_mean = Mean(female_foot_sizes, 4);
        female_foot_std = StandardDev(female_foot_sizes, 4, female_foot_mean);
    }
    
    double Probability(double x, double mean, double std_dev) {
        if (std_dev == 0) return 1.0;
        double exponent = -pow((x - mean) / std_dev, 2) / 2;
        return (1 / (std_dev * sqrt(2 * PI))) * exp(exponent);
    }
    
    string Gender(double height, double weight, double foot_size) {
        double male_height_prob = Probability(height, male_height_mean, male_height_std);
        double male_weight_prob = Probability(weight, male_weight_mean, male_weight_std);
        double male_foot_prob = Probability(foot_size, male_foot_mean, male_foot_std);
        
        double female_height_prob = Probability(height, female_height_mean, female_height_std);
        double female_weight_prob = Probability(weight, female_weight_mean, female_weight_std);
        double female_foot_prob = Probability(foot_size, female_foot_mean, female_foot_std);
        
        double male_total_prob = male_height_prob * male_weight_prob * male_foot_prob;
        double female_total_prob = female_height_prob * female_weight_prob * female_foot_prob;
        
        if (male_total_prob > female_total_prob) {
            return "Male";
        } else {
            return "Female";
        }   
     }
    
   
   
};

int main() {
    // Create classifier and train
    GenderClassifier classifier;
    classifier.trainModel();
    
   
    
    double height, weight, foot_size;
   

        cout<<"Enter the following details to predict"<<endl;
        cout<<"Note!: Enter height in ft, weight in lbs and foot size in inches"<<endl;
        cout << "\nEnter Height (ft): ";
        cin >> height;
        
        cout << "Enter Weight (lbs): ";
        cin >> weight;
        
        cout << "Enter Foot Size (inches): ";
        cin >> foot_size;
        
        string prediction = classifier.Gender(height, weight, foot_size);
        cout << "Predicted Gender: " << prediction << endl;
        
    
    return 0;
}