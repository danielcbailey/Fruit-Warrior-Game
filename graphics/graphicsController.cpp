

#include "graphicsController.hpp"
#include <cstdint>
#include <cstdlib>
#include "bitmapImage.hpp"
#include "globals.hpp"

GraphicsController* _globalGraphics;

//testing
void printRoi(GraphicsElement::RegionOfInfluence roi) {
    printfdbg("x: %d y: %d x2: %d, y2: %d\n", roi.x, roi.y, roi.x2, roi.y2);
}

void GraphicsController::renderAllAbove(GraphicsElement* element, bool add) {
    //getting the layer of the passed element and its region of influence

    int elemLayer = element->getLayer();
    GraphicsElement::RegionOfInfluence roi = element->getROI();

    //getting to the layer
    std::list<std::list<GraphicsElement*>>::iterator it = this->elements.begin();

    int layerNum = -1;

    while (it != this->elements.end()) {
        std::list<GraphicsElement*> layer = *it;
        if (layer.size() == 0) {
            ++it;
            continue;
        }

        GraphicsElement* elem = *layer.begin();
        
        layerNum = elem->getLayer();

        if (elemLayer <= layerNum) {
            break;
        }

        it++;
    }

    std::list<GraphicsElement*> layer;

    if (add) {
        if (layerNum == -1 || layerNum != elemLayer) {
            //must make a layer for the element to reside in
            it = elements.insert(it, std::list<GraphicsElement*>());
        }

        //Adding the element to the layer
        (*it).push_back(element);
    }

    //Rendering everything that overlaps the affected region in this layer or higher layers
    for (; it != elements.end(); ++it) {
        layer = *it;

        for (std::list<GraphicsElement*>::iterator iti = layer.begin(); iti != layer.end(); ++iti) {
            GraphicsElement* elem = *iti;

            if (elem->isInROI(roi)) {
                //Needs to be rendered again
                elem->renderWithin(this->lcd, roi);
            }
        }
    }
}

void GraphicsController::renderAllIn(GraphicsElement::RegionOfInfluence roi) {
    if ((roi.x > roi.x2) || (roi.y > roi.y2)) {
        //An invalid roi
        return;
    }

    //Rendering everything that overlaps the affected region in all layers
    for (std::list<std::list<GraphicsElement*>>::iterator it = elements.begin(); it != elements.end(); ++it) {
        std::list<GraphicsElement*> layer = *it;

        for (std::list<GraphicsElement*>::iterator iti = layer.begin(); iti != layer.end(); ++iti) {
            GraphicsElement* elem = *iti;

            if (elem->isInROI(roi)) {
                //Needs to be rendered again

                if (elem->getType() == GraphicsElement::ElementType::ELEMENT_IMAGE) {
                    //if the image is large enough and ROI is small enough, can resample image
                    //Primarily aimed at ensuring the background image doesn't get redrawn

                    if (elem->getContext()->Image.width * elem->getContext()->Image.height > (roi.x2 - roi.x + 1) * (roi.y2 - roi.y + 1) &&
                        (roi.x2 - roi.x + 1) * (roi.y2 - roi.y + 1) < 1024) {
                            //resample
                            GraphicsElement* temp = GraphicsElement::resampleImage(roi, elem);
                            temp->render(this->lcd); //it is known that it is contained within the ROI
                            //free(temp->getContext()->Image.image->getRawBuffer());
                            delete(temp->getContext()->Image.image);
                            delete(temp);
                            continue;
                        }
                }
                elem->renderWithin(this->lcd, roi);
            }
        }
    }
}

uint16_t GraphicsController::samplePixel(int x, int y, int layerSample) {
    //Not the most efficient method, but iterates the fewest times over the layers list
    //In almost all cases, it will be sampling from the background, so it is well optimized for
    //this use case.

    if (x < 0 || x > 127 || y < 0 || y > 127) {
        return 0;
    }

    uint16_t ret = 0;
    GraphicsElement::RegionOfInfluence roi = {x, y, x, y};
    std::list<std::list<GraphicsElement*>>::iterator it = this->elements.begin();
    while (it != this->elements.end()) {
        std::list<GraphicsElement*> layer = *it;
        if (layer.size() == 0) {
            ++it;
            continue;
        }

        GraphicsElement* first = *layer.begin();

        if (first->getLayer() >= layerSample) {
            break;
        }

        for (std::list<GraphicsElement*>::iterator iti = layer.begin(); iti != layer.end(); ++iti) {
            GraphicsElement* elem = *iti;

            if (elem->isInROI(roi)) {
                //Needs to be rendered again
                elem->sample(x, y, &ret);
            }
        }

        ++it;
    }

    return ret;
}

void GraphicsController::registerGraphicsElement(GraphicsElement *element) {
    renderAllAbove(element, true); //will add the element to the layer system and render it immediately
    element->setRenderedROI(element->getROI());
}

void GraphicsController::updateGraphicsElement(GraphicsElement *element) {
    GraphicsElement::RegionOfInfluence oldRoi = element->getRenderedROI();
    GraphicsElement::RegionOfInfluence roi = element->getROI();

    //Calculating if there is overlap between the new and old location
    if (!element->isInROI(oldRoi)) {
        //No overlap, can simply render without the element in the old location and render it in the new location
        renderAllIn(oldRoi);
        renderAllAbove(element, false);
        element->setRenderedROI(element->getROI());
        return;
    }

    //The non-trivial case where there is overlap and there must be two subregions formed of what used to be occupied
    //by the element that moved

    //The top/bottom regions extend to the full width of the former ROI, the left/right ones do not extend to the full
    //height of the former ROI
    GraphicsElement::RegionOfInfluence sideRoi;
    GraphicsElement::RegionOfInfluence tbRoi; //tb for top-bottom roi since I can't think of a single word for it
    bool renderSide = true;
    bool renderTb = true;

    if (roi.x < oldRoi.x) {
        //Moved left
        sideRoi.x = roi.x2 + 1;
        sideRoi.x2 = oldRoi.x2;

        //defaulting y coords to be based on new position
        sideRoi.y = roi.y;
        sideRoi.y2 = roi.y2;
    } else if (roi.x > oldRoi.x) {
        //Moved right
        sideRoi.x2 = roi.x - 1;
        sideRoi.x = oldRoi.x;

        //defaulting y coords to be based on new position
        sideRoi.y = roi.y;
        sideRoi.y2 = roi.y2;
    } else {
        renderSide = false;
    }

    if (roi.y < oldRoi.y) {
        //Moved up
        tbRoi.y = roi.y2 + 1;
        tbRoi.y2 = oldRoi.y2;
        sideRoi.y = oldRoi.y;
        sideRoi.y2 = roi.y2;
    } else if (roi.y > oldRoi.y) {
        //Moved down
        tbRoi.y2 = roi.y - 1;
        tbRoi.y = oldRoi.y;
        sideRoi.y = roi.y;
        sideRoi.y2 = oldRoi.y2;
    } else {
        //No movement
        renderTb = false;
    }

    tbRoi.x = oldRoi.x;
    tbRoi.x2 = oldRoi.x2;

    //The renderAllIn function will check for invalid ROIs
    if (renderSide) {
        renderAllIn(sideRoi);
    }

    if (renderTb) {
        renderAllIn(tbRoi);
    }
    renderAllAbove(element, false);
    element->setRenderedROI(element->getROI());
}

void GraphicsController::removeGraphicsElement(GraphicsElement *element) {
    std::list<std::list<GraphicsElement*>>::iterator it = this->elements.begin();

    while (it != this->elements.end()) {
        
        std::list<GraphicsElement*> layer = *it;
        if (layer.size() == 0) {
            ++it;
            continue;
        }

        GraphicsElement* elem = *layer.begin();
        
        if (element->getLayer() != elem->getLayer()) {
            //layer not found
            ++it;
            continue;
        }

        for (std::list<GraphicsElement*>::iterator iti = layer.begin(); iti != layer.end(); ++iti) {
            elem = *iti;

            if (elem == element) {
                (*it).remove(elem);
                renderAllIn(element->getRenderedROI());
                return;
            }
        }

        ++it;
    }
}

void GraphicsController::registerAnimation(AnimationPlayer* player) {
    this->animations.push_back(player);
}

void GraphicsController::removeAnimation(AnimationPlayer* player) {
    this->animations.remove(player);
}

void GraphicsController::handleGraphicsTick(float dt) {
    for (std::list<AnimationPlayer*>::iterator it = this->animations.begin(); it != this->animations.end(); ++it) {
        BitmapImage* frame = (*it)->getFrame(dt);
        GraphicsElement* elem = (*it)->getElement();

        if (elem->getType() != GraphicsElement::ElementType::ELEMENT_IMAGE) {
            //shouldn't happen, but just in case
            continue;
        }

        if (elem->getContext()->Image.image == frame) {
            //no change, so can just skip
            continue;
        }

        //A change occured, must update the element then have the controller handle the update
        elem->getContext()->Image.image = frame;

        //updating context size
        elem->getContext()->Image.width = frame->getWidth();
        elem->getContext()->Image.height = frame->getHeight();

        this->updateGraphicsElement(elem);
    }
}

void GraphicsController::refresh() {
    this->renderAllIn(this->globalBoundary);
}