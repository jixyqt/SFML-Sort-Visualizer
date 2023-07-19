#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#define endl "\n"

using namespace sf;


const int NumElements = 800;

class Visualizer {
private:
    const unsigned int WindowWidth = 800;
    const unsigned int WindowHeight = 600;
    const float RectangleWidth = 2.f;
    const float RectangleHeight = 178.f;
    const Color BackgroundColor = Color::Black;
    const Color DefaultRectangleColor = Color::Cyan;
    const Color CurrentRectangleColor = Color::Red;
    const std::string TextureFile = "Resources/fondo1.png";

    RenderWindow appWindow;
    Event appEvent;
    Texture fondoTexture;
    Sprite fondoSprite;
    RectangleShape rectangle;
    Font font;
    Text timerText;
    Text sortText;
    Text swapText;
    
    bool bl = false;
    int randomh = 0;
    int itera = 0;
    int key = 0;
    int part = 1;
    int min_idx;
    int lims = NumElements - 1;
    struct data {
        int height;
        int posx;
    } line[NumElements];

    struct blank {
        int height;
        int posx;
    } blanks[NumElements];

    void loadResources();
    void setupRectangle();
    void processEvents();
    void drawRectangles();
    void resetRectangles();
    void setupBlanks();
    void stopSorting();
    bool isSorted();
    void exitEvent(bool*);

public:
    Visualizer();
    void homeScreen();
    void selectionSort();
    void bubbleSort();
    void insertionSort();
    void mergeSort();
    void merge(data array[], int const left, int const mid,
        int const right);
    void mergeHelper(data array[], int const begin, int const end);
    int partition(data array[], int const low, int const high);
    void quickHelper(data array[], int low, int high);
    void quickSort();
};

Visualizer::Visualizer() {
    
    loadResources();
    setupRectangle();
    setupBlanks();

    if (!font.loadFromFile("Resources/font.ttf")) {
        throw std::runtime_error("Failed to load font: Resources/font.ttf");
    }

    timerText.setFont(font);
    timerText.setFillColor(Color::Blue);
    timerText.setCharacterSize(20);
    timerText.setPosition(Vector2f(0.f, 10.f));

    sortText.setFont(font);
    sortText.setFillColor(Color::Blue);
    sortText.setCharacterSize(20);
    sortText.setPosition(Vector2f(0.f, 30.f));

    swapText.setFont(font);
    swapText.setFillColor(Color::Blue);
    swapText.setCharacterSize(20);
    swapText.setPosition(Vector2f(0.f, 50.f));
}

void Visualizer::loadResources() {
    if (!fondoTexture.loadFromFile(TextureFile)) {
        throw std::runtime_error("Failed to load texture: " + TextureFile);
    }
    fondoSprite.setTexture(fondoTexture);
}

void Visualizer::setupRectangle() {
    rectangle.setSize(Vector2f(RectangleWidth, RectangleHeight));
    rectangle.setFillColor(DefaultRectangleColor);
    rectangle.setRotation(180.f);
}

void Visualizer::processEvents() {
    while (appWindow.pollEvent(appEvent)) {
        if (appEvent.type == Event::Closed) {
            appWindow.close();
        }
    }
}

void Visualizer::drawRectangles() {
    appWindow.clear(BackgroundColor);
    appWindow.draw(fondoSprite);
    if (!bl) {
        for (int i = 0; i < NumElements; i++) {
            rectangle.setFillColor(DefaultRectangleColor);
            if (i == itera) {
                rectangle.setFillColor(CurrentRectangleColor);
            }
            rectangle.setPosition((float)line[i].posx, (float)WindowHeight);
            rectangle.setSize(Vector2f(RectangleWidth, (float)line[i].height));
            appWindow.draw(rectangle);
        }
    }
    else {
        for (int i = 0; i < NumElements; i++) {
            rectangle.setFillColor(DefaultRectangleColor);
            if (i == itera) {
                rectangle.setFillColor(CurrentRectangleColor);
            }
            rectangle.setPosition((float)blanks[i].posx, (float)WindowHeight);
            rectangle.setSize(Vector2f(RectangleWidth, (float)blanks[i].height));
            appWindow.draw(rectangle);
        }
    }

    appWindow.draw(timerText);
    appWindow.draw(sortText);
    appWindow.draw(swapText);
    appWindow.display();
}

void Visualizer::resetRectangles() {
    part = 1;
    randomh = 0;
    itera = 0;
    key = 0;
    lims = NumElements - 1;
    bl = true;
}

void Visualizer::setupBlanks() {
    for (int i = 0; i < NumElements; i++) {
        blanks[i].posx = i;
        blanks[i].height = 1;
    }
}


void Visualizer::stopSorting() {
    lims = itera;
    itera = 0;
}

bool Visualizer::isSorted() {
    for (int i = 0; i < NumElements - 1; i++) {
        if (line[i].height > line[i + 1].height) {
            return false;
        }
    }
    return true;
}

void Visualizer::exitEvent(bool* sortingComplete) {
    if (isSorted()) {
        stopSorting();
        *sortingComplete = true;
        std::chrono::seconds duration(3);
        std::this_thread::sleep_for(duration);
        appWindow.close();
    }
}

void Visualizer::mergeHelper(data array[], int const begin, int const end) {
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeHelper(array, begin, mid);
    mergeHelper(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void Visualizer::merge(data array[], int const left, int const mid,
    int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;
    int swaps = 0;

    // Create temp arrays
    auto* leftArray = new int[subArrayOne],
        * rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = line[left + i].height;
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = line[mid + 1 + j].height;

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne
        && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            line[indexOfMergedArray].height
                = leftArray[indexOfSubArrayOne]; swaps++;
            indexOfSubArrayOne++;
        }
        else {
            line[indexOfMergedArray].height
                = rightArray[indexOfSubArrayTwo]; swaps++;
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        line[indexOfMergedArray].height
            = leftArray[indexOfSubArrayOne]; swaps++;
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        line[indexOfMergedArray].height
            = rightArray[indexOfSubArrayTwo]; swaps++;
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    swapText.setString("Merges:" + std::to_string(swaps));

    drawRectangles();

    delete[] leftArray;
    delete[] rightArray;
}


void Visualizer::mergeSort() {
    appWindow.create(VideoMode(WindowWidth, WindowHeight), "Sorting Algorithm Visualizer || Merge Sort");

    int cont = 0, sorts = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool sortingComplete = false;
    resetRectangles(); bl = false;

    while (appWindow.isOpen() && !sortingComplete) {
        processEvents();
        sortText.setString("Iterations: " + std::to_string(cont + 1));

        if (part == 1) {
            for (int i = 0; i < NumElements; i++) {
                randomh = 1 + rand() % WindowHeight;
                line[i].posx = i;
                line[i].height = randomh;
                if (i == NumElements - 1) part = 2;
            }
        }

        if (part == 2) mergeHelper(line, itera, NumElements);

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        timerText.setString("Time: " + std::to_string(elapsedTime) + " secs");
        
        itera++;
        if (itera >= lims) {
            itera = 0;
            cont++;
            lims--;
        }

        exitEvent(&sortingComplete);
    }
}

int Visualizer::partition(data array[], int const low, int const high) {
    int pivot = array[high].height;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j].height < pivot) {
            i++; std::swap(array[i].height, array[j].height);
        }
    }
    std::swap(array[i + 1].height, array[high].height);
    
    drawRectangles();

    return (i + 1);
}

void Visualizer::quickHelper(data array[], int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);

        quickHelper(array, low, pi - 1);
        quickHelper(array, pi + 1, high);
    }
}

void Visualizer::quickSort() {
    appWindow.create(VideoMode(WindowWidth, WindowHeight), "Sorting Algorithm Visualizer || Quick Sort");

    int cont = 0, sorts = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool sortingComplete = false;
    resetRectangles(); bl = false;

    while (appWindow.isOpen() && !sortingComplete) {
        processEvents();

        if (part == 2) quickHelper(line, itera, NumElements);
        
        if (part == 1) {
            for (int i = 0; i < NumElements; i++) {
                randomh = 1 + rand() % WindowHeight;
                line[i].posx = i;
                line[i].height = randomh;
                if (i == NumElements - 1) part = 2;
            }
        }
        
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        timerText.setString("Time: " + std::to_string(elapsedTime) + " secs");
        sortText.setString("Iterations: " + std::to_string(cont));
        swapText.setString("Swaps:" + std::to_string(sorts));
       
        itera++;
        if (itera >= lims) {
            itera = 0;
            cont++;
            lims--;
        }

        exitEvent(&sortingComplete);

    }
}

void Visualizer::selectionSort() {
    appWindow.create(VideoMode(WindowWidth, WindowHeight), "Sorting Algorithm Visualizer || Selection Sort");

    int cont = 0, sorts = 0;
    auto startTime = std::chrono::steady_clock::now();
    bool sortingComplete = false;
    resetRectangles(); bl = false;

    while (appWindow.isOpen() && !sortingComplete) {
        processEvents();

        min_idx = itera;
        if (part == 2) {
            for (int i = itera + 1; i < NumElements; i++) {
                if (line[i].height < line[min_idx].height)
                    min_idx = i;
            }
        }

        if (min_idx != itera) {
            std::swap(line[min_idx].height, line[itera].height);
            sorts++;
        }

        if (part == 1) {
            for (int i = 0; i < NumElements; i++) {
                randomh = 1 + rand() % WindowHeight;
                line[i].posx = i;
                line[i].height = randomh;
                if (i == NumElements - 1) part = 2;
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        timerText.setString("Time: " + std::to_string(elapsedTime) + " secs");
        sortText.setString("Iterations: " + std::to_string(cont));
        swapText.setString("Swaps:" + std::to_string(sorts));

        drawRectangles();

        itera++;
        if (itera >= lims) {
            itera = 0;
            cont++;
            lims--;
        }

        exitEvent(&sortingComplete);
    }
}

void Visualizer::insertionSort() {
    appWindow.create(VideoMode(WindowWidth, WindowHeight), "Sorting Algorithm Visualizer || Insertion Sort");

    int cont = 0, sorts=0;
    auto startTime = std::chrono::steady_clock::now();
    bool sortingComplete = false;
    resetRectangles(); bl = false;

    while (appWindow.isOpen() && !sortingComplete) {
        processEvents();

        key = line[itera].height;
        int j = itera - 1;

        if (part == 2) {
            while (j >= 0 && line[j].height > key) {
                line[j + 1].height = line[j].height;
                j = j - 1; sorts++;
            }
        }
        line[j + 1].height = key;

        if (part == 1) {
            for (int i = 0; i < NumElements; i++) {
                randomh = 1 + rand() % WindowHeight;
                line[i].posx = i;
                line[i].height = randomh;
                if (i == NumElements - 1) part = 2;
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        timerText.setString("Time: " + std::to_string(elapsedTime) + " secs");
        sortText.setString("Iterations: " + std::to_string(cont));
        swapText.setString("Insertions:" + std::to_string(sorts));

        drawRectangles();

        itera++;
        if (itera >= lims) {
            itera = 0;
            cont++;
            lims--;
        }
        
        exitEvent(&sortingComplete);
    }
}

void Visualizer::bubbleSort() {
    appWindow.create(VideoMode(WindowWidth, WindowHeight), "Sorting Algorithm Visualizer || Bubble Sort");

    int cont = 0, sorts=0;
    auto startTime = std::chrono::steady_clock::now();
    bool sortingComplete = false;
    resetRectangles(); bl = false;

    while (appWindow.isOpen() && !sortingComplete) {
        processEvents();

        if (part == 2) {
            if (line[itera].height < line[itera + 1].height) {
                std::swap(line[itera].height, line[itera + 1].height);
                sorts++;
            }
        }

        if (part == 1) {
            for (int i = 0; i < NumElements; i++) {
                randomh = 1 + rand() % WindowHeight;
                line[i].posx = i;
                line[i].height = randomh;
                if (i == NumElements - 1) part = 2;
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        timerText.setString("Time: " + std::to_string(elapsedTime) + " secs");
        sortText.setString("Iterations: " + std::to_string(cont));
        swapText.setString("Swaps:" + std::to_string(sorts));

        drawRectangles();

        itera++;
        if (itera >= lims) {
            itera = 0;
            cont++;
            lims--;
        }

        exitEvent(&sortingComplete);
    }
}

void Visualizer::homeScreen() {
    RenderWindow startWindow(VideoMode(WindowWidth, WindowHeight), "Sort Algorithm Visualizer || Home Screen");

    Font font;
    if (!font.loadFromFile("Resources/font.ttf")) {
        throw std::runtime_error("Failed to load font: Resources/font.ttf");
    }

    Text text;
    text.setFont(font);
    text.setFillColor(Color::Cyan);
    text.setStyle(Text::Bold);
    text.setCharacterSize(20);
    text.setString("Press 'A' for Bubble Sort\nPress 'B' for Insertion Sort\nPress 'C' for Selection Sort\nPress 'D' for Merge Sort\nPress 'E' for Quick Sort\nPress 'F' for Heap Sort\nPress 'H' for Tim Sort");
    text.setPosition((float)startWindow.getSize().x / 2 - 175.f, (float)startWindow.getSize().y / 2 - 100.f);

    while (startWindow.isOpen()) {
        Event Event;
        while (startWindow.pollEvent(Event)) {
            switch (Event.type) {
                // window closed
            case Event::Closed:
                startWindow.close();
                break;

                // key pressed
            case Event::TextEntered:
                if (Event.text.unicode == 65 || Event.text.unicode == 97) {
                    sleep(seconds(2.0f));
                    bubbleSort();   
                }
                if (Event.text.unicode == 66 || Event.text.unicode == 98) {
                    sleep(seconds(2.0f));
                    insertionSort(); 
                 
                }
                if (Event.text.unicode == 67 || Event.text.unicode == 99) {
                    sleep(seconds(2.0f));
                    selectionSort();
                    
                }
                if (Event.text.unicode == 68 || Event.text.unicode == 100) {
                    sleep(seconds(2.0f));
                    mergeSort();
                }
                if (Event.text.unicode == 69 || Event.text.unicode == 101) {
                    sleep(seconds(2.0f));
                    quickSort();
                }
                break;

                // we don't process other types of events
            default:
                break;
            }
        }
        startWindow.clear();
        startWindow.draw(text);
        startWindow.display();
    }
}

int main() {
    Visualizer program;
    program.homeScreen();

    return 0;
}
