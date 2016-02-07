#include "GameDrawer.hpp"

sf::Vector2f GameDrawer::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

sf::Color GameDrawer::B2toSF(const b2Color& col) const
{
	return sf::Color(
		255U * col.r,
		255U * col.g,
		255U * col.b,
		255U * col.a);
}

sf::Color GameDrawer::tweakAlpha(const sf::Color & col, int alpha) const
{
	return sf::Color(col.r, col.g, col.b, alpha);
}

sf::Color GameDrawer::blend(const sf::Color& a, int aParts, const sf::Color& b, int bParts) const
{
	
	int totalParts = aParts + bParts;

	sf::Color total(
		(a.r * aParts + b.r * bParts) / totalParts,
		(a.g * aParts + b.g * bParts) / totalParts,
		(a.b * aParts + b.b * bParts) / totalParts);

		return total;
}

GameDrawer::GameDrawer(sf::RenderWindow& win, GameWorld* world) : Drawer(win), world_(world) 
{
}

void GameDrawer::draw()
{
	drawBounds(world_->getBounds());

	std::list<Side*>& sides = world_->getSides();
	for (Side* sd : sides)
	{
		drawSide(sd);
	}

	std::list<Projectile*>& projs = world_->getProjectiles();
	for (Projectile* prj : projs)
	{
		drawProjectile(prj);
	}

	std::list<Enemy*>& shapes = world_->getShapes();
	for (Enemy* shp : shapes)
	{
		drawShape(shp);
	}

	Shape* player = world_->getPlayer();
	if (player != nullptr)
	{
		drawShape(player);
	}

}

void GameDrawer::drawShape(Shape* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(s->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();
	float size = s->getSize();
	float angle = body->GetAngle();

	sf::Vector2f pointing(sin(angle), -cos(angle));
	pointing *= (float)(_SCALE_ * 1.5);
	
	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = B2toSF(body->GetLinearVelocity(), true);
	vel *= (float)(size * _SCALE_ * 2);
	
	sf::Color pri = B2toSF(s->getPrimary());
	sf::Color sec = B2toSF(s->getSecondary());
	sf::Color ter = B2toSF(s->getTertiary());

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		
		verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
	}

	bool armed = s->getArmed();
	if (armed)
	{
		armed = s->getWeaponReady();
	}

	//Draw
	//Velocity line
	drawLine(pos, pos - vel, ter);

	//Actual shape
	drawPolygon(verts, count, pri, sec);

	//Centre circle
	drawCircle(pos, size * 4, sec, sec, 0);
	
	//Orientation circle
	drawCircle(pos - pointing, 4, (armed ? ter : tweakAlpha(ter, 128)), (armed ? sec : tweakAlpha(sec, 128)));
	
	//sf::Vector2f* arrow = new sf::Vector2f[3];
	//arrow[1] = (pos - pointing);
	//arrow[2] = (pos - sf::Vector2f(-pointing.x, pointing.y / 2));
	//arrow[3] = (pos - sf::Vector2f(pointing.x, pointing.y / 2));
	//
	//drawPolygon(arrow, 3, ter, sec);

	//Clean up
	delete[] verts;
}

void GameDrawer::drawBounds(Bounds* const b)
{
	//Pull vars
	b2Body* body = b->getBody();
	b2Fixture* fix = body->GetFixtureList();
	
	sf::Vector2f pos = B2toSF(b->getPosition(), true);

	sf::Color pri = B2toSF(b->getPrimary());
	sf::Color sec = B2toSF(b->getSecondary());
	sf::Color ter = B2toSF(b->getTertiary());
	
	//It must be one of two
	if (fix->GetUserData() != "bounds")
	{
		fix = fix->GetNext();
	}

	//Pull other vars
	b2ChainShape* shape = static_cast<b2ChainShape*>(fix->GetShape());
	int count = shape->m_count;

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		verts[i] = B2toSF(body->GetWorldPoint(shape->m_vertices[i]), true);
	}

	//Draw layered bounds below
	sf::Vector2f* subVerts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		subVerts[i] = verts[i];
	}

	int layers = 8;
	for (int l = 0; l <= layers; ++l)
	{
		for (int v = 0; v < count; ++v)
		{
			subVerts[v] *= 0.8f;
		}

		drawPolygon(subVerts, count, tweakAlpha(blend(ter, l + 1, pri, layers - 2), l * 8), sec);
	}


	//Draw stuff
	drawPolygon(verts, count, tweakAlpha(pri, 128), sec);

	drawCircle(pos, 1, ter, pri);

	//Clean up
	delete[] verts;
}

void GameDrawer::drawProjectile(Projectile* const p)
{
	//Pull vars
	b2Body* body = p->getBody();

	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = B2toSF(body->GetLinearVelocity(), true);
	vel *= (float)_SCALE_;

	sf::Color pri = B2toSF(p->getPrimary());
	sf::Color sec = B2toSF(p->getSecondary());
	sf::Color ter = B2toSF(p->getTertiary());
	
	//This will cause problems once projectiles stop being only circles
	b2CircleShape* shape = static_cast<b2CircleShape*>(p->getBody()->GetFixtureList()->GetShape());
	float rad = shape->m_radius * _SCALE_;

	//Draw shape, vel, pos
	drawLine(pos, pos - vel, ter);
	drawCircle(pos, rad, pri, sec);
	drawPoint(pos, ter);
}

void GameDrawer::drawSide(Side* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	float length = s->getValue();
	sf::Vector2f pos = B2toSF(s->getPosition(), true);
	sf::Vector2f dir = B2toSF(s->getHeading(), true);

	sf::Color pri = B2toSF(s->getPrimary());
	sf::Color sec = B2toSF(s->getSecondary());
	sf::Color ter = B2toSF(s->getTertiary());

	b2Fixture* fix = body->GetFixtureList();

	//It must be one of two
	if (fix->GetUserData() != "side")
	{
		fix = fix->GetNext();
	}

	//Pull other vars
	b2EdgeShape* shape = static_cast<b2EdgeShape*>(fix->GetShape());

	sf::Vector2f a = B2toSF(body->GetWorldPoint(shape->m_vertex1), true);
	sf::Vector2f b = B2toSF(body->GetWorldPoint(shape->m_vertex2), true);

	//Draw line
	drawLine(a, b, pri);
	drawCircle(pos, thor::length(dir), tweakAlpha(pri, 16), tweakAlpha(sec, 32));
	//drawCircle(pos, length, sec, pri, 0);
}

/*GameDrawer::DrawShape(const &Shape s)
{
	s.draw(this);
}
*/

