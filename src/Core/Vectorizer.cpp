#include <vector>
#include <Vectorizer/Vectorizer.h>
#include "IO/ImageLoader.h"
#include "Vectorizer/util.h"

namespace Vectorizer
{
    using EdgePair = std::pair<int, int>;
    static const List<EdgePair> marchingSquaresLUT[16] = {
        {},                     // Case 0: ----
        { {0, 3} },             // Case 1: #---
        { {1, 0} },             // Case 2: -#--
        { {1, 3} },             // Case 3: ##--
        { {2, 1} },             // Case 4: --#-
        { {0, 3}, {2, 1} },     // Case 5: #-#-
        { {2, 0} },             // Case 6: -##-
        { {2, 3} },             // Case 7: ###-
        { {3, 2} },             // Case 8: ---#
        { {0, 2} },             // Case 9: #--#
        { {1, 0}, {3, 2} },     // Case 10: -#-#
        { {1, 2} },             // Case 11: #-##
        { {3, 1} },             // Case 12: --##
        { {0, 1} },             // Case 13: #.##
        { {3, 0} },             // Case 14: -###
        {}                      // Case 15: ####
    };

    bool isSolid(int x, int y, int width, int height, int channels, const unsigned char* data)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            return false;
        }

        int index = (y * width + x) * channels;

        return data[index] < 128;
    }

    void simplifyRecursive(const Math::Chain& originalChain, size_t startIndex, size_t endIndex, float tolerance, Math::Chain& outChain)
    {
        float maxDistance = 0.0f;
        size_t farthestIndex = startIndex;
        Math::Segment segment = { originalChain[startIndex], originalChain[endIndex] };

        for (size_t i = startIndex + 1; i < endIndex; ++i)
        {
            float currentDistance = pointToSegmentDistance(segment, originalChain[i]);
            if (currentDistance > maxDistance)
            {
                maxDistance = currentDistance;
                farthestIndex = i;
            }
        }

        if (maxDistance > tolerance)
        {
            simplifyRecursive(originalChain, startIndex, farthestIndex, tolerance, outChain);
            simplifyRecursive(originalChain, farthestIndex, endIndex, tolerance, outChain);
        }
        else
        {
            outChain.push_back(originalChain[endIndex]);
        }
    }

    //Ramer-Douglas-Peucker
    Math::Chain simplifyChain(Math::Chain& chain, float tolerance)
    {
        if (chain.size() < 3) {
            return chain;
        }

        Math::Chain simplifiedChain;
        simplifiedChain.push_back(chain.front());

        simplifyRecursive(chain, 0, chain.size() - 1, tolerance, simplifiedChain);
        chain = simplifiedChain;
        return chain;
    }

    List<Math::Chain> buildChainsFromSegments(List<Math::Segment>& segments)
    {
        List<Math::Chain> chains = {};
        while (segments.size() > 1)
        {
            Math::Chain chain = { segments.back().start, segments.back().end };
            segments.pop_back();
            bool foundNext = true;
            while (foundNext)
            {
                foundNext = false;
                for (auto iter = segments.begin(); iter != segments.end();)
                {
                    if (iter->start == chain.back())
                    {
                        chain.push_back(iter->end);
                        iter = segments.erase(iter);
                        foundNext = true;
                        break;
                    }
                    else
                    {
                        ++iter;
                    }
                }
            }
            if (chain.size() > 20) {
                chains.push_back(chain);
            }
        }
        return chains;
    }

    List<Math::Segment> marchingSquares(ImageData image)
    {
        List<Math::Segment> allSegments;

        for (int y = -1; y < image.height; ++y)
        {
            for (int x = -1; x < image.width; ++x)
            {
                bool topLeft = isSolid(x, y, image.width, image.height, image.channels, image.data);
                bool topRight = isSolid(x + 1, y, image.width, image.height, image.channels, image.data);
                bool bottomLeft = isSolid(x, y + 1, image.width, image.height, image.channels, image.data);
                bool bottomRight = isSolid(x + 1, y + 1, image.width, image.height, image.channels, image.data);

                int index = 0;
                if (topLeft) index += 1;
                if (topRight) index += 2;
                if (bottomRight) index += 4;
                if (bottomLeft) index += 8;

                if (index == 0 || index == 15) {
                    continue;
                }
                const auto& rules = marchingSquaresLUT[index];

                for (const auto& rule : rules)
                {
                    Math::Point startPoint, endPoint;

                    switch (rule.first) //starting point
                    {
                    case 0: startPoint = { x + 0.5f, (float)y }; break; //top
                    case 1: startPoint = { x + 1.f, y + 0.5f }; break; //right
                    case 2: startPoint = { x + 0.5f, y + 1.f }; break; //bottom
                    case 3: startPoint = { (float)x, y + 0.5f }; break; //left
                    }
                    switch (rule.second) //ending point
                    {
                    case 0: endPoint = { x + 0.5f, (float)y }; break; //top
                    case 1: endPoint = { x + 1.f, y + 0.5f }; break; //right
                    case 2: endPoint = { x + 0.5f, y + 1.f }; break; //bottom
                    case 3: endPoint = { (float)x, y + 0.5f }; break; //left
                    }
                    allSegments.push_back({ startPoint, endPoint });
                }
            }
        }
        return allSegments;
    }
    void printChainsToConsole(List<Math::Chain>& chains, int imageWidth, int imageHeight, int consoleWidth)
    {
        if (chains.empty())
        {
            std::cout << "Nessuna catena da visualizzare." << std::endl;
            return;
        }

        int consoleHeight = static_cast<int>((static_cast<float>(imageHeight) / imageWidth) * consoleWidth * 0.5f);

        std::vector<std::string> canvas(consoleHeight, std::string(consoleWidth, '.'));

        char drawChar = 'A';

        for (const auto& chain : chains)
        {
            for (const auto& point : chain)
            {
                int x = static_cast<int>((point.x / imageWidth) * consoleWidth);
                int y = static_cast<int>((point.y / imageHeight) * consoleHeight);

                if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight)
                {
                    canvas[y][x] = drawChar;
                }
            }
            drawChar++;
            if (drawChar > 'Z') drawChar = 'A';
        }

        std::cout << "\n--- Visualizzazione Contorni Semplificati ---" << std::endl;
        for (int i = 0; i < consoleHeight; ++i)
        {
            std::cout << canvas[i] << std::endl;
        }
        std::cout << "---------------------------------------------\n" << std::endl;
    }
    List<Math::Chain> vectorizeImage(std::string path, float tolerance)
    {
        ImageData image = ImageLoader::loadImageData(path);
        if (!image.isValid())
        {
            std::cerr << "Error: can't load image" << std::endl;
            return List<Math::Chain>{};
        }

        List<Math::Segment> rawSegments = Vectorizer::marchingSquares(image);

        List<Math::Chain> chains = Vectorizer::buildChainsFromSegments(rawSegments);

        for (Math::Chain& chain : chains)
        {
            Vectorizer::simplifyChain(chain, tolerance);
        }
        return chains;
    }
}
