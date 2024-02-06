#include <iostream>
#include <algorithm>

typedef struct num {
    int numerator;
    int denominator;
} num;

num mod(num number)
{   
    if (number.numerator == 0)
    {
        return number;
    }

    if(number.numerator < 0 && number.denominator > 0)
    {
        number.numerator = number.numerator * -1;
        return number;
    }

    if(number.numerator > 0 && number.denominator < 0)
    {
        number.denominator = number.denominator * -1;
        return number;
    }

    if(number.numerator < 0 && number.denominator < 0)
    {
        number.numerator = number.numerator * -1;
        number.denominator = number.denominator * -1;
        return number;
    }

    return number;
}

bool biggerThan(num lhs, num rhs)
{
    lhs = mod(lhs);
    rhs = mod(rhs);

    if (lhs.numerator * rhs.denominator > rhs.numerator * lhs.denominator)
        return true;
    else
        return false;
}

num inverse(num number)
{
   if(number.numerator != 0)
    {
            int temp = number.numerator;
            number.numerator = number.denominator;
            number.denominator = temp;
    }
    return number;
}

void simplify (num* number)
{
    int GCD = std::__gcd(number->numerator, number->denominator);
    number->denominator = number->denominator / GCD;
    number->numerator = number->numerator / GCD;
}

num multiply(num n1, num n2)
{
    num result;

    result.numerator = n1.numerator * n2.numerator;
    result.denominator = n1.denominator * n2.denominator;

    simplify(&result);

    return result;
}

num divide(num numer, num denom)
{
    denom = inverse(denom);
    return multiply(numer, denom);
}

num add(num n1, num n2)
{
    num result;
    result.denominator = n1.denominator * n2.denominator;
    result.numerator = n1.numerator * n2.denominator + n2.numerator * n1.denominator;

    simplify(&result);
    return result;
}

num subtract(num lhs, num rhs)
{
    rhs.numerator = rhs.numerator * -1;
    return add(lhs, rhs);
}

void printarr(num** arr, int row){
    for (int i = 0; i < row; i++){
        std::cout << "| ";
        for (int j = 0; j < (row + 1); j++)
        {
            if (arr[i][j].denominator == 1)
            {
                if(j == 0)
                    std::cout << std::noshowpos << " " << arr[i][j].numerator << " * x" << j + 1;
                else if (j == row)
                    std::cout << std::noshowpos << " = " << arr[i][j].numerator;
                else
                    std::cout << std::showpos << " " << arr[i][j].numerator << " * x" << std::noshowpos << j+1;
            }
            else 
            {
                if(j == 0)
                    std::cout << std::noshowpos << " " << ((arr[i][j].denominator > 0) ? arr[i][j].numerator : ((arr[i][j].numerator > 0) ? (arr[i][j].numerator * -1) : arr[i][j].numerator))<< "/" << std::noshowpos << ((arr[i][j].denominator > 0) ? arr[i][j].denominator : (arr[i][j].denominator * -1)) << " * x" << j+1;
                else if (j == row)
                    std::cout << std::noshowpos << " = " << ((arr[i][j].denominator > 0) ? arr[i][j].numerator : ((arr[i][j].numerator > 0) ? (arr[i][j].numerator * -1) : arr[i][j].numerator))<< "/" << std::noshowpos << ((arr[i][j].denominator > 0) ? arr[i][j].denominator : (arr[i][j].denominator * -1)) ;
                else
                    std::cout << std::showpos << " " << ((arr[i][j].denominator > 0) ? arr[i][j].numerator : ((arr[i][j].numerator > 0) ? (arr[i][j].numerator * -1) : arr[i][j].numerator))<< "/" << std::noshowpos << ((arr[i][j].denominator > 0) ? arr[i][j].denominator : (arr[i][j].denominator * -1))  << " * x" << j+1;
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void swapRow(num** arr, int columns, int rowindex1, int rowindex2)
{
    num temp;
    for (int i = 0; i < columns; i++)
    {
        temp = arr[rowindex1][i];
        arr[rowindex1][i] = arr[rowindex2][i];
        arr[rowindex2][i] = temp;
    }

}

int GaussForwardStep(num** arr, int row, int modified)
{
    int columns = row + 1;
    for (int i = 0; i < row - 1; i++)
    {
        if (arr[i][i].numerator == 0)
        {   
            int flag = 1;
            for (int pivotFind = 1; pivotFind < row; pivotFind++)
            {
                if (arr[pivotFind][0].numerator == 0)
                    continue;
                else
                {
                    swapRow(arr, columns, 0, pivotFind);
                    flag = 0;
                    break;
                }
            }
            if (flag)
            {
                std::cout << "The matrix has no solutions (det == 0)";
                return 0;
            }
        }
        if (modified && i < (row - 1))
        {
            num biggest = arr[i][i];
            int rowindexmodified = i;

            for (int k = i; k < row; k++)
            {
                if(biggerThan(arr[k][i], biggest))
                {
                    biggest = arr[k][i];
                    rowindexmodified = k;
                }
            }
            if(rowindexmodified != i)
            {
                swapRow(arr, columns, i, rowindexmodified);
                std::cout << "Swapped rows " << i+1 << " and " << rowindexmodified+1 << "\n";
                printarr(arr, row);
            }

        }
        num placeholder = arr[i][i];
        for (int j = 0; j < columns; j++)
        {
            arr[i][j] = divide(arr[i][j], placeholder);
        }

        std::cout << "Post-Normalization SOLE\n";
        printarr(arr, row);

        for (int k = i + 1; k < row; k++)
        {
            num div = divide(arr[k][i], arr[i][i]);
            for (int j = 0; j < columns; j++)
            {
                arr[k][j] = subtract(arr[k][j], multiply(div, arr[i][j]));
            }
        }
    }
    std::cout << "Square SOLE after forwards-Gauss: \n";
    printarr(arr, row);
    for (int i = 0; i < row; i++)
    {
        if(arr[i][i].numerator == 0)
        {
            std::cout << "The matrix has no solutions (det == 0)";
            return 0;
        }
    }
        return 1;
}

void GaussBackwardsStep(num** arr, int row)
{
    int columns = row + 1;
    for (int i = row - 1; i >-1; i--)
    {
            num placeholder = inverse(arr[i][i]);
            for (int j = i; j < columns; j++)
            {
                arr[i][j] = multiply(arr[i][j], placeholder);
            }
        if (i != 0)
        {
            for (int j = i; j > 0; j--)
            {
                if(arr[j-1][i].numerator != 0)
                {
                    arr[j - 1][columns - 1] = subtract(arr[j - 1][columns - 1], multiply(arr[j - 1][i], arr[i][columns - 1]));
                }
                arr[j - 1][i].numerator = 0;
            }

        }
        
    }

    std::cout << "Solution Matrix: \n";
    printarr(arr, row);
}

int Gauss(num** arr, int row, int modifiedflag){
    int returncodecheck = 1;
    returncodecheck = GaussForwardStep(arr, row, modifiedflag);
    if(returncodecheck) 
        GaussBackwardsStep(arr, row);
    else
        return 0;
    return 1;
}

int main(){
    std::cout << "Input the number of equations!\n";

    int row = 0;

    std::cin >> row;

    int column = row + 1;

    num **arr = new num*[row];

    for (int i = 0; i < row; i++)
    {
        arr[i] = new num[column];
    }

    int temp1 = 0;
    int temp2 = 0;
    int fractionFlag = 2;
    int modifiedflag = 0;
    std::cout << "\n\nModidied Gauss Method?\n\n\n1) Yes\n\n2) No\n\n";
    std::cin >> modifiedflag;
    modifiedflag = (modifiedflag == 1) ? 1 : 0;

    std::cout << "\n\nDo you want to input the coefficients in the fraction form?\n\n\n1) Yes\n\n2) No\n\n";
    std::cin >> fractionFlag;

    
    for (int i = 0; i < row; i++)
    {
        temp1 = i + 1;
        for (int j = 0; j < column; j++){
            temp2 = j + 1;
            if(fractionFlag == 1)
            {
                if(j == column - 1)
                {
                    std::cout << "Eq." << temp1 << ") Input the solution's numerator.\n";
                    std::cin >> arr[i][j].numerator;
                    std::cout << "Eq." << temp1 << ") Input the solution's denomerator.\n";
                    std::cin >> arr[i][j].denominator;
                }
                else
                {
                    std::cout << "Eq." << temp1 << ") Input the numerator of the coefficient before the x #" << temp2 <<".\n";
                    std::cin >> arr[i][j].numerator;
                    std::cout << "Eq." << temp1 << ") Input the denominator of the coefficient before the x #" << temp2 <<".\n";
                    std::cin >> arr[i][j].denominator;
                }
                if (arr[i][j].denominator == 0)
                {
                    std::cout << "Invalid denominator Input! Stopping...";
                   
                    return 0;
                }
            }
            else
            {
                if(j == column - 1)
                {
                    std::cout << "Eq." << temp1 << ") Input the solution for the equation.\n";
                    std::cin >> arr[i][j].numerator;
                }
                else
                {
                    std::cout << "Eq." << temp1 << ") Input the coefficient before the x #" << temp2 <<".\n";
                    std::cin >> arr[i][j].numerator;
                }
                arr[i][j].denominator = 1;
            }
        }
    }

    std::cout << "Provided square SOLE: \n";
    printarr(arr, row);

    if(Gauss(arr, row, modifiedflag))
    {
        std::cout << "Solutions for the SOLE: \n";
        for (int i = 0; i < row; i++)
        {
            if(arr[i][row].denominator == 1)
                std::cout << "x" << i + 1 << " = " << arr[i][row].numerator; // row - last column of the matrix i know its not intuitive so what
            else // help
                std::cout << "x" << i + 1 << " = " << ((arr[i][row].denominator > 0) ? arr[i][row].numerator : ((arr[i][row].numerator > 0) ? (arr[i][row].numerator * -1) : arr[i][row].numerator))<< "/" << std::noshowpos << ((arr[i][row].denominator > 0) ? arr[i][row].denominator : (arr[i][row].denominator * -1)) << std::endl; 
        }
    }

    for (int i = 0; i < row; i++)
    {
        delete arr[i];
    }
    delete arr;

    return 0;
}