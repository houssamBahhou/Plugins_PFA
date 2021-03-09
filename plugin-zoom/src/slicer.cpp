#include "CImg.h"
#include <iostream>
#include <getopt.h>
#include <map>
#include <vector>
#include <cmath>
#include <omp.h>
#include <ctime>

#define TIME_DIFF(t1, t2) \
        ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec))

#define logger(m) cout << m << endl;

using namespace cimg_library;
using namespace std;

typedef CImg<unsigned int> image_t;

typedef map<string, image_t> imageMap_t;

//global for commandline option
char *filename;
string gregion;
size_t depth = -1;
size_t size = 400;

//global map
imageMap_t imMap;

//global for thread
int parallelLevel;
int parallelDepth = 1;


void usage() {
    cout << "Usage :" << endl;
    cout << "-i : image to process" << endl;
    cout << "-r : The id of the gregion to construct (if not chosen nothing is construct)" << endl;
    cout << "-l : the level of details to slice the image" << endl;
    cout << "-s : the size of the displayed image" << endl;
    cout << "-p : the parallel depth (set to 1 by default)" << endl;
    cout << "Example :" << endl;
    cout << "./slicer -i image/test.jpg -l2 -r0" << endl;
}

void parse_opts(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "i:s:l:r:p:")) != -1) {
        switch (opt) {
            case 'r':
                gregion = optarg;
                break;
            case 'l':
                depth = strtol(optarg, nullptr, 0);
                break;
            case 's':
                size = strtol(optarg, nullptr, 0);
                break;
            case 'i':
                filename = optarg;
                break;
            case 'p':
                parallelDepth = strtol(optarg, nullptr, 0);
                break;
            default: /* '?' */
                usage();
                exit(EXIT_FAILURE);
        }
    }
}

/**
 * Slice an image
 * @param beginx is where the slice begin at x
 * @param beginy is where the slice begin at y
 * @param sliceWidth is the slice width
 * @param sliceHeigth is the slice height
 * @param im is the image to slice
 * @return a slice of im
 */
image_t slice(size_t beginx, size_t beginy, size_t sliceWidth, size_t sliceHeigth,
              image_t &im) {
    size_t width = im.width();
    size_t height = im.height();

    size_t x, y;
    if (beginx + sliceWidth < width)
        x = sliceWidth;
    else
        x = width - beginx;
    if (beginy + sliceHeigth < height)
        y = sliceHeigth;
    else
        y = height - beginy;

    CImg<unsigned char> newImage(x, y, im.depth(), im.spectrum()); //constructor
    cimg_forXYZC(newImage, i, j, z, c) {
                    newImage(i, j, z, c) = im(beginx + i, beginy + j, z, c);
                }
    return newImage;
}

int power(int x, int n) {
    int res = 1;
    for(int i = 0; i < n; i++) {
        res = res * x;
    }
    return res;
}

/**
 * Create tile and store them in a map
 * @param im the original image
 * @param levelOfDetails the level of details to slice to image
 * @param id is the id of the slice
 */
void createTiles(image_t &im, int levelOfDetails, const string &id = "") {
    if (levelOfDetails == 0) {
        #pragma omp critical
        imMap.insert({id, im});
        return;
    }

    size_t width = im.width();
    size_t height = im.height();

    string newId;
    #pragma omp parallel for collapse(2) private(newId) if (levelOfDetails > parallelLevel - parallelDepth)
    for (size_t i = 0; i < width; i += width / 2 + 1) {
        for (size_t j = 0; j < height; j += height / 2 + 1) {

            if (i < width / 2 + 1 && j < height / 2 + 1)
                newId = id + to_string(0);
            else if (i >= width / 2 + 1 && j < height / 2 + 1)
                newId = id + to_string(1);
            else if (i < width / 2 + 1 && j >= height / 2 + 1)
                newId = id + to_string(2);
            else if (i >= width / 2 + 1 && j >= height / 2 + 1)
                newId = id + to_string(3);

            image_t tile = slice(i, j, width / 2, height / 2, im);
            createTiles(tile, levelOfDetails - 1, newId);

        }
    
    }

}

void createKey(vector<image_t> &v, vector<string> &keys) {
    for (size_t i = 0; i < v.size(); i++) {
        imMap.insert({keys[i], v[i]});
    }
}

void createTilesIter(image_t &im, int levelOfDetails) {
    vector<image_t> v;
    v.push_back(im);

    vector<string> keys;
    keys.push_back("");

    int width = im.width();
    int height = im.height();

    for(int l = 0; l < levelOfDetails; l++) {
        vector<image_t> tmp;
        vector<string> tmpKeys;
        
        #pragma omp parallel for collapse(3) shared(tmp, tmpKeys)
        for (size_t k = 0; k < v.size(); k++) {
            for (size_t i = 0; i < width; i += width / 2 + 1) {
                for (size_t j = 0; j < height; j += height / 2 + 1) {

                    image_t tile = slice(i, j, width / 2, height / 2, v[k]);
                    #pragma omp critical
                    {
                        tmp.push_back(tile);
            
                        if (i < width / 2 + 1 && j < height / 2 + 1)
                            tmpKeys.push_back(keys[k] + to_string(0));
                        else if (i >= width / 2 + 1 && j < height / 2 + 1)
                            tmpKeys.push_back(keys[k] + to_string(1));
                        else if (i < width / 2 + 1 && j >= height / 2 + 1)
                            tmpKeys.push_back(keys[k] + to_string(2));
                        else if (i >= width / 2 + 1 && j >= height / 2 + 1)
                            tmpKeys.push_back(keys[k] + to_string(3));
                    }
                }
            }
        }
        v = tmp;
        keys = tmpKeys;

        width /= 2;
        height /= 2;
    }
    createKey(v, keys);
}

/**
 * check if a key is in the region to construct
 * @param regionToConstruct is the id of the region to construct
 * @param key is the tested quadkey
 * @return return true if the key belong the region to construct
 */
bool checkKey(const string &regionToConstruct, const string &key) {
    for (int i = 0; i != regionToConstruct.size(); ++i) {
        if ((regionToConstruct != "-1") && (key[i] != regionToConstruct[i]))
        return false;
    }
    return true;
}
/**
 * construct a region
 * @param im is the original image
 * @return return an image of the region
 */
image_t constructRegion(array<image_t, 4> im) {
    int xsize = im[0].width() + im[1].width();
    int ysize = im[0].height() + im[2].height();

    CImg<unsigned char> newImage(xsize, ysize, im[0].depth(), im[0].spectrum());
    int indexx = 0;
    int indexy = 0;
    for (int a = 0; a < 4; ++a) {
        cimg_forXYZC(im[a], i, j, z, c) {
                        newImage(indexx + i, indexy + j, z, c) = im[a](i, j, z, c);
                    }
        if (a == 0)
            indexx = im[a].width();
        if (a == 1) {
            indexx = 0;
            indexy = im[a].height();
        }
        if (a == 2) {
            indexx = im[a].width();
            indexy = im[a - 1].height();
        }
    }
    return newImage;
}

/**
 * get all image from a region and launch the construction
 * @param region the region to construct
 */
void construct(const string &region) {

    //pour le calcule du temps d'execution
    struct timeval t1, t2;
    gettimeofday(&t1,NULL);

    vector<array<image_t, 4>> vregion;

    int count = 0;
    int index = -1;
    for (auto &it : imMap) {
        if (checkKey(region, it.first)) {
            if (count % 4 == 0) {
                vregion.emplace_back(array<image_t, 4>{it.second});
                ++index;
            } else {
                vregion[index][count % 4] = it.second;
            }
            ++count;
        }
    }

    vector<image_t> tmp;
    while (vregion.size() >= 4) {
        #pragma omp parallel for shared(tmp) ordered schedule(guided)
        for (int i = 0; i < vregion.size(); i++) {
            image_t imRegion = constructRegion(vregion[i]);
            
            #pragma omp ordered
            tmp.emplace_back(imRegion);
        }
        vregion.clear();
        for (int i = 0; i < tmp.size(); i += 4)
            vregion.push_back(array<image_t, 4>{tmp[i], tmp[i + 1], tmp[i + 2], tmp[i + 3]});
        tmp.clear();
    }
    //pour le calcule du temps d'execution
    gettimeofday(&t2,NULL);
    unsigned long temps = TIME_DIFF(t1,t2);
    printf("time(construction) = %ld ms\n", temps/1000);

    constructRegion(vregion[0]).resize(size, size).display();
}

int computeLevel (int size, image_t im) {
    size_t width = im.width();
    size_t height = im.height();

    int dim = (width >= height) ? width : height;
    int div = dim / size;

    return (int) floor(log2(div)) + 1;
}

int main(int argc, char **argv) {
    parse_opts(argc, argv);
    omp_set_nested(1);

    if (filename == nullptr) {
        usage();
        exit(EXIT_FAILURE);
    }

    //Load image from disk
    image_t image(filename);

    struct timeval t1, t2;
    gettimeofday(&t1,NULL);
    if (depth == -1) {
        depth = computeLevel(size, image);
    }

    parallelLevel = depth;

    createTiles(image, depth);
    //createTilesIter(image, depth);

    gettimeofday(&t2,NULL);

    unsigned long temps = TIME_DIFF(t1,t2);

    printf("time(creation) = %ld ms\n", temps/1000);

    if (!gregion.empty()) {
        construct(gregion);
    }
    return EXIT_SUCCESS;
}