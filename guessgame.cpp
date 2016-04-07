//  Dylan Lynch
//  11/9/15
//  MY FIRST C++ PROGRAM!
//  guessgame.cpp
//  game to guess a number between 0 and 100


#include <iostream>
#include <cmath>
#include <ctime>
#include <limits>

using namespace std;


int main (){

    int answer;
    int guess = 0;
    bool success = 0, error = 0;


    srand(time(0));
    answer = rand() % 101;

    cout << "Guess an integer between 0 and 100" << endl;

    while (success == false){

        // get number
        cin >> guess;

        // check if there's stuff left in stream
        if (cin.get() != '\n') {
            cout << "Please input an integer" << endl;
            error = 1;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }




        //checks if number is within bounds
        if ((guess < 0 || guess > 100) && error != 1){
            cout << "Please pick an integer between 0 and 100" << endl;
            error = 1;
        }

        //checks number vs answer
        if ((guess == answer) && error != 1){
            cout << "You win!" << endl;
            success = 1;
        }
        else {
            if ((guess > answer && error != 1)){
                cout << "Too high" << endl;
            }
            else if ((guess < answer && error != 1)){
                cout << "Too low" << endl;
            }
        }


        error = 0; //reset error check

    } //end of while loop

    return 0;
}
