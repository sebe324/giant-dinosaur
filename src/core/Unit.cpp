#include "Unit.h"

Unit::Unit(std::string n, sf::Vector2f p, sf::Vector2f s, float spd) {
    speed = spd;
    hitbox = sf::Rect<float>(p.x,p.y,s.x,s.y);
    bodyParts = sf::VertexArray(sf::Triangles,0);
}
Unit::Unit() {};

sf::Vector2f Unit::getCenter() const{
    return {hitbox.left + hitbox.width * 0.5f, hitbox.top + hitbox.height * 0.5f};
}

void Unit::moveUp(sf::Time elapsed, float multiplier) {
    velocity.y -= speed * elapsed.asSeconds() * multiplier;
}
void Unit::moveDown(sf::Time elapsed, float multiplier) {
    velocity.y += speed * elapsed.asSeconds() * multiplier;
}
void Unit::moveLeft(sf::Time elapsed, float multiplier) {
    velocity.x -= speed * elapsed.asSeconds() * multiplier;
    if (velocity.x < 0 && !left) {
        look();
    }
}
void Unit::moveRight(sf::Time elapsed, float multiplier) {
    velocity.x += speed * elapsed.asSeconds() * multiplier;
    if (velocity.x > 0 && left) {
        look();
    }
}
void Unit::move() {
    velocity.y *= 0.9;
    velocity.x *= 0.9;
    for (int i = 0; i < bodyPartsNumber * 6; i++) {
        bodyParts[i].position += velocity;
    }
    hitbox.top += velocity.y;
    hitbox.left += velocity.x;
}

void Unit::look() {
    for (int i = 0; i < bodyPartsNumber * 6; i++) {
        bodyParts[i].position.x = Utils::mirrorXf(bodyParts[i].position.x,hitbox.left,hitbox.left+hitbox.width);
    }
    left = !left;
}

float Unit::getDistance(Unit& unit) const{
    return sqrt(pow(unit.getCenter().x-getCenter().x,2)+pow(unit.getCenter().y-getCenter().y,2));
}
float Unit::getDistanceX(Unit& unit) const{
    return getCenter().x-unit.getCenter().x;
}

float Unit::getDistanceY(Unit& unit) const{
    return getCenter().y-unit.getCenter().y;
}

void Unit::rotate(float angle) {
    sf::Transform t;
    t.rotate(angle,getCenter());
    for (int i=0; i<bodyPartsNumber*6; i++) {
        bodyParts[i].position=t.transformPoint(bodyParts[i].position);
    }
}
void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(bodyParts,states);
}
void Unit::createBodyPart(sf::Vector2f s, sf::Vector2f offset, sf::Color color) {
    bodyPartsNumber++;
    bodyParts.resize(bodyPartsNumber*6);
    bodyParts[(bodyPartsNumber*6)-1].position={hitbox.left+offset.x,hitbox.top+offset.y};
    bodyParts[(bodyPartsNumber*6)-1].color=color;
    bodyParts[(bodyPartsNumber*6)-2].position={hitbox.left+offset.x+s.x,hitbox.top+offset.y};
    bodyParts[(bodyPartsNumber*6)-2].color=color;
    bodyParts[(bodyPartsNumber*6)-3].position={hitbox.left+offset.x+s.x,hitbox.top+offset.y+s.y};
    bodyParts[(bodyPartsNumber*6)-3].color=color;
    
    bodyParts[(bodyPartsNumber * 6) - 4].position = { hitbox.left + offset.x,hitbox.top + offset.y};
    bodyParts[(bodyPartsNumber * 6) - 4].color = color;
    bodyParts[(bodyPartsNumber * 6) - 5].position = { hitbox.left + offset.x,hitbox.top + offset.y+s.y};
    bodyParts[(bodyPartsNumber * 6) - 5].color = color;
    bodyParts[(bodyPartsNumber * 6) - 6].position = { hitbox.left + offset.x+s.x,hitbox.top + offset.y +s.y};
    bodyParts[(bodyPartsNumber * 6) - 6].color = color;


}
