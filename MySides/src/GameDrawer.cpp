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

GameDrawer::GameDrawer(sf::RenderWindow& win, GameWorld* world) : Drawer(win), world_(world) 
{
}

void GameDrawer::draw()
{
	drawBounds(world_->getBounds());

	Shape* player = world_->getPlayer();
	if (player != nullptr)
	{
		drawShape(player);
	}

	std::list<Enemy*>& shapes = world_->getShapes();
	for (Enemy* shp : shapes)
	{
		drawShape(shp);
	}

	std::list<Projectile*>& projs = world_->getProjectiles();
	for (Projectile* prj : projs)
	{
		drawProjectile(prj);
	}

	std::list<Side*>& sides = world_->getSides();
	for (Side* sd : sides)
	{
		drawSide(sd);
	}
}

//Deprecated, remove?
void GameDrawer::drawPlayer(Shape* const p)
{
	//Pull vars
	b2Body* body = p->getBody();
	b2Vec2 pos = body->GetWorldCenter();
	b2Vec2 vel = pos + body->GetLinearVelocity();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(p->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();
	sf::Color pri = B2toSF(p->getPrimary());
	sf::Color sec = B2toSF(p->getSecondary());
	sf::Color ter = B2toSF(p->getTertiary());

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
	}

	//Draw shape, vel, pos
	drawPolygon(verts, count, pri, sec);

	drawLine(B2toSF(pos, true), B2toSF(vel, true), ter);

	drawCircle(B2toSF(pos, true), 1, sec, pri); // Fix off center

	drawPoint(B2toSF(pos, true), sf::Color::Red);

	//Clean up
	delete[] verts;
}

void GameDrawer::drawShape(Shape* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(s->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();
	
	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = pos + B2toSF(s->getBody()->GetLinearVelocity(), true);
	
	sf::Color pri = B2toSF(s->getPrimary());
	sf::Color sec = B2toSF(s->getSecondary());
	sf::Color ter = B2toSF(s->getTertiary());

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		
		verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
	}

	//Draw shape, vel, pos
	drawPolygon(verts, count, pri, sec);
	
	drawLine(pos, vel, ter);

	drawCircle(pos, 1, sec, pri); // Fix off center
	
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

		drawPolygon(subVerts, count, sf::Color(pri.r, pri.g, pri.b, l * 8), sec);
	}


	//Draw stuff
	drawPolygon(verts, count, sf::Color(pri.r, pri.g, pri.b, 128), sec);

	drawCircle(pos, 1, ter, pri);

	//Clean up
	delete[] verts;
}

void GameDrawer::drawProjectile(Projectile* const p)
{
	//Pull vars
	b2Body* body = p->getBody();

	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = pos + B2toSF(p->getBody()->GetLinearVelocity(), true);

	sf::Color pri = B2toSF(p->getPrimary());
	sf::Color sec = B2toSF(p->getSecondary());
	sf::Color ter = B2toSF(p->getTertiary());
	
	//This will cause problems once projectiles stop being only circles
	b2CircleShape* shape = static_cast<b2CircleShape*>(p->getBody()->GetFixtureList()->GetShape());
	float rad = shape->m_radius * _SCALE_;

	//Draw shape, vel, pos
	drawCircle(pos, rad, pri, sec);
	drawLine(pos, vel, ter);
	drawPoint(pos, sec);
}

void GameDrawer::drawSide(Side* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	sf::Vector2f pos = B2toSF(s->getPosition(), true);
	sf::Vector2f vel = pos + B2toSF(s->getBody()->GetLinearVelocity(), true);

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
	drawPoint(pos, sec);
}

/*GameDrawer::DrawShape(const &Shape s)
{
	s.draw(this);
}
*/

