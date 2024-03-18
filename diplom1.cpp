#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <ctime>
#include <opencv2/opencv.hpp>

//#define v vector

using namespace std;

//Основные функции
int*** getmask(int* H, int* W, int* count, string address);
sf::Color*** genSample(int count, int H, int W);
sf::Image genImage(int*** mask, sf::Color*** sample, int count, int H, int W);
cv::VideoWriter genVideo(sf::Image image);

//Доп функции
sf::Color** genSampleZebra(int H, int W);
sf::Color** genSampleCircle(int H, int W);
sf::Color** genSampleSquare(int H, int W);


int main()
{
    string address;
    int* H, * W, * count= new int();
    srand(time(0));

    int*** mask = getmask(ref(H), ref(W), ref(count), address);

    sf::Color*** sample = genSample(*count, *H, *W);

    sf::Image image = genImage(mask, sample, *count, *H, *W);

    //Нужен opencv для видео
}

int*** getmask(int* H, int* W, int* count, string address) {
    //v<v<v<int>>> mask;
    int*** mask;
    ifstream file(address, ios::in | ios::binary); // Открываем файл в бинарном режиме

    if (file.is_open()) {
        
        // Определяем размер файла
        *count = file.get();
        *H = file.get();
        *W = file.get();
        int fileSize = (*H) * (*W);

        // Выделяем память для хранения данных файла
        char* buffer = new char[fileSize];

        // Чтение данных в буфер
        file.read(buffer, fileSize);

        // Выводим содержимое буфера (ваша обработка байтов данных)
        for (int i = 0; i < fileSize; ++i) {
            cout << buffer[i];
        }
        /*
        //Способ через векторы
        v<int> tempRow;
        v<v<int>> tempLayer;
        for (int i = 0; i < *count; i++) {
            
            for (int j = 0; j < *H; j++) {
                for (int k = 0; k < *W; k++) {
                    tempRow.push_back(buffer[k + (*H * j) + ((*H) * (*W) * i)]);
                }
                tempLayer.push_back(tempRow);
            }
            mask.push_back(tempLayer);
        }
        */
        //Способ через динамический массив
        int*** mass1 = new int** [*count];
        //Каждый слой
        for (int i = 0; i < *count; i++) {
            mass1[i] = new int* [*W];
            for (int j = 0; j < *W; j++) {
                mass1[i][j] = new int[*H];
                for (int k = 0; k < *H; k++) {
                    k = buffer[k + (*H * j) + ((*H) * (*W) * i)];
                }
            }
        }
            
        // Освобождаем ресурсы
        delete[] buffer;
    }
    else {
        cerr << "Ошибка открытия файла";
    }

    file.close(); // Закрываем файл

    return mask;
}

sf::Color*** genSample(int count, int H, int W) {

    // Создаем двумерный массив для хранения пикселей
    sf::Color*** samples = new sf::Color ** [count];

    //for (unsigned int x = 0; x < W; ++x)
    //{
    //    pixels[x] = new sf::Color[H];
    //    for (unsigned int y = 0; y < H; ++y)
    //    {
    //        pixels[x][y] = templateImage.getPixel(x, y);
    //    }
    //}

    for (unsigned int i = 0; i < count; i++) {
        sf::Color** samples = new sf::Color * [H];
        for (int j = 0; j < H; j++) {
            sf::Color* samples = new sf::Color[W];
        }
    }

    for (int i = 0; i < count; i++) {
        switch (count % 3) {
        case 0:
            samples[i] = genSampleZebra(H, W);
            break;
        case 1:
            samples[i] = genSampleSquare(H, W);
            break;
        default:
            samples[i] = genSampleCircle(H, W);
            break;
        }
    }
    return samples;
}

sf::Color** genSampleZebra(int H, int W) {
    sf::Color** samp = new sf::Color * [H];
    for (int i = 0; i < H; H++)
        samp[i] = new sf::Color[W];

    sf::Color color1(rand() % 255, rand() % 255, rand() % 255);
    sf::Color color2(rand() % 255, rand() % 255, rand() % 255);//Могут быть одинаковыми

    int lineW = W / (rand()%20+5);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (j % (lineW * 2)<lineW)
                samp[i][j] = color1;
            else
                samp[i][j] = color2;
        }
    }
    return samp;
}

sf::Color** genSampleCircle(int H, int W) {
    sf::Color** samp = new sf::Color * [H];
    for (int i = 0; i < H; H++)
        samp[i] = new sf::Color[W];

    sf::Color color1(rand() % 255, rand() % 255, rand() % 255);
    sf::Color color2(rand() % 255, rand() % 255, rand() % 255);//Могут быть одинаковыми

    int between = W / (rand()%20+5);
    int way = 1;//rand() % 2;

    /*if (way == 1) {
        int r = rand() % (between / 2) + 1;

        for (int i = r; i < H; i += between) {
            for (int j = r; j < W; j += between) {
                
            }
        }
    }*/
    
    sf::Image image;
    image.create(W, H, color2);
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    sf::CircleShape circle(50); // Радиус круга
    circle.setFillColor(color1); // Цвет круга
    circle.setPosition(100, 100); // Позиция круга на изображении

    sf::RenderTexture renderTexture;
    renderTexture.create(W, H);
    renderTexture.clear();
    renderTexture.draw(sprite);

    int r = rand() % (between / 2) + 1;
    for (int i = r; i < H; i += between) {
        for (int j = r; j < W; j += between) {
            circle.setPosition(j, i);
            renderTexture.draw(circle);
        }
    }

    renderTexture.display();

    sf::Image resultImage = renderTexture.getTexture().copyToImage();

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            samp[i][j] = image.getPixel(i, j);
        }
    }
    
    return samp;
}

sf::Color** genSampleSquare(int H, int W) {
    sf::Color** samp = new sf::Color * [H];
    for (int i = 0; i < H; H++)
        samp[i] = new sf::Color[W];

    sf::Color color1(rand() % 255, rand() % 255, rand() % 255);
    sf::Color color2(rand() % 255, rand() % 255, rand() % 255);//Могут быть одинаковыми

    sf::Image image;
    image.create(W, H, color2);
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    sf::RectangleShape square(sf::Vector2f(100,100)); // Радиус круга
    square.setFillColor(color1); // Цвет круга
    square.setPosition(100, 100); // Позиция круга на изображении

    sf::RenderTexture renderTexture;
    renderTexture.create(W, H);
    renderTexture.clear();
    renderTexture.draw(sprite);
    
    int between = W / (rand() % 20 + 5);
    int radius = between - 1 / 2; //пока что будет так

    for (int i = radius; i < H; i += between) {
        for (int j = radius; j < W; j += between) {
            square.setPosition(j, i);
            renderTexture.draw(square);
        }
    }

    renderTexture.display();

    sf::Image resultImage = renderTexture.getTexture().copyToImage();

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            samp[i][j] = image.getPixel(i, j);
        }
    }

    return samp;
}

sf::Image genImage(int*** mask, sf::Color*** samp, int count, int H, int W) {
    sf::Image resultImage;

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < H; j++) {
            for (int k = 0; k < W; k++) {
                if (mask[i][j][k])
                    resultImage.setPixel(j, k, samp[i][j][k]);
            }
        }
    }
    return resultImage;
}

cv::VideoWriter genVideo(sf::Image image) {
    cv::VideoWriter video("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, image.getSize());
    cv::VideoWriter vi1()
}