// timsort.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>

using namespace std;
const int RUN = 32; 

class List
{
public:
    ~List();

    List();

    void clear();

    void insert(int index, int value);

    void removeAt(int index);

    void pop_front();

    void pop_back();

    void push_front(int data);

    void push_back(int data);

    int GetSize() { return size; }

    void PrintList(List list);

    long long& operator[](const int index);

private:

    class Node {
    public:
        Node* pnext;
        long long data;
        Node(int data = -1, Node* pnext = nullptr)
        {
            this->data = data;
            this->pnext = pnext;
        }
    };
    int size;
    Node* head;
};

List::List()
{
    size = 0;
    head = nullptr;
}

List::~List()
{

}

void List::push_back(int data) {
    if (head == nullptr)
    {
        head = new Node(data);
    }
    else
    {
        Node* current = head;
        while (current->pnext != nullptr)
        {
            current = current->pnext;
        }
        current->pnext = new Node(data);
    }
    size++;
}

void List::pop_back()
{
    removeAt(size - 1);
}

void List::push_front(int data)
{
    head = new Node(data, head);
    size++;
}

void List::removeAt(int index)
{
    if (index == 0)
        pop_front();
    else
    {
        Node* current = this->head;
        for (int i = 0; i < index - 1; i++)
            current = current->pnext;
        Node* toDelete = current->pnext;
        current->pnext = toDelete->pnext;
        delete toDelete;
        size--;
    }

}

void List::insert(int index, int value)
{

    int counter = 0;
    Node* current = this->head;
    while (index != counter && current != nullptr)
    {
        current = current->pnext;
        counter++;
    };
    Node* temp = new Node(value, current->pnext);
    current->pnext = temp;
    size++;
}

void List::pop_front()
{
    Node* temp = head;

    head = head->pnext;

    delete temp;

    size--;
}

void List::clear()
{
    while (size)
        pop_front();
}

long long& List::operator[](const int index)
{
    int counter = 0;
    Node* current = this->head;
    while (current != nullptr)
    {
        if (index == counter)
            return current->data;
        current = current->pnext;
        counter++;
    }
}

void List::PrintList(List list) {
    for (int i = 0; i < size; i++)
        cout << "   " << list[i] << "\n";

}

// This function sorts array from left index to
// to right index which is of size atmost RUN
// A binary search based function
// to find the position
// where item should be inserted
// in a[low..high]
int binarySearch(int a[], int item,
    int low, int high)
{
    if (high <= low)
        return (item > a[low]) ?
        (low + 1) : low;

    int mid = (low + high) / 2;

    if (item == a[mid])
        return mid + 1;

    if (item > a[mid])
        return binarySearch(a, item,
            mid + 1, high);
    return binarySearch(a, item, low,
        mid - 1);
}

// Function to sort an array a[] of size 'n'
void insertionSort(int a[], int n)
{
    int i, loc, j, k, selected;

    for (i = 1; i < n; ++i)
    {
        j = i - 1;
        selected = a[i];

        // find location where selected should be inseretd
        loc = binarySearch(a, selected, 0, j);

        // Move all elements after location to create space
        while (j >= loc)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}

// Merge function merges the sorted runs
void merge(int arr[], int l, int m, int r)
{
    // Original array is broken in two parts
    // left and right array
    int len1 = m - l + 1,len2 = r - m;
    int *left=new int[len1], *right=new int[len2];
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    int i = 0;
    int j = 0;
    int k = l;

    // After comparing, we
    // merge those two array
    // in larger sub array
    while (i < len1 && j < len2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left, if any
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }

    // Copy remaining element of right, if any
    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
    }
}

// Iterative Timsort function to sort the
// array[0...n-1] (similar to merge sort)
void timSort(int arr[], int n)
{

    // Sort individual subarrays of size RUN
    for (int i = 0; i < n; i += RUN)
        insertionSort(arr, n);

    // Start merging from size RUN (or 32).
    // It will merge
    // to form size 64, then 128, 256
    // and so on ....
    for (int size = RUN; size < n;size = 2 * size)
    {

        // pick starting point of
        // left sub array. We
        // are going to merge
        // arr[left..left+size-1]
        // and arr[left+size, left+2*size-1]
        // After every merge, we
        // increase left by 2*size
        for (int left = 0; left < n;left += 2 * size)
        {

            // find ending point of
            // left sub array
            // mid+1 is starting point
            // of right sub array
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1),(n - 1));

            // merge sub array arr[left.....mid] &
            // arr[mid+1....right]
            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

// Utility function to print the Array
void printArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d  ", arr[i]);
    printf("\n");
}

// Driver program to test above function
int main()
{
    int arr[] = { -2, 7, 15, -14, 0 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Given Array is\n");
    printArray(arr, n);

    // Function Call
    timSort(arr, n);

    printf("After Sorting Array is\n");
    printArray(arr, n);
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
