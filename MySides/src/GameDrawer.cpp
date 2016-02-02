#include "GameDrawer.hpp"

sf::Vector2f GameDrawer::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

sf::Color GameDrawer::B2toSF(const b2Color& col) const
{
	return sf::Color(col.r, col.g, col.b, col.a);
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
		drawPlayer(player);
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

	////Get a reference or friend function or what have you
	//for (Shape s : world_->shapes_)
	//{
	//
	//}
	//
	//for (Projectile p : world_->projectiles_)
	//{
	//
	//}
}

void GameDrawer::drawPlayer(Shape* const p)
{
	//Pull vars
	b2Body* body = p->getBody();
	b2Vec2 pos = body->GetWorldCenter();
	b2Vec2 vel = pos + body->GetLinearVelocity();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(p->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
	}

	//Draw shape, vel, pos
	drawPolygon(verts, count, sf::Color(0, 128, 0), sf::Color(0, 128,0));

	drawLine(B2toSF(pos, true), B2toSF(vel, true), sf::Color::Black);

	drawCircle(B2toSF(pos, true), 2); // Fix off center

	drawPoint(B2toSF(pos, true), sf::Color::Red);

	//Clean up
	delete[] verts;
}

void GameDrawer::drawShape(Shape* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	b2Vec2 pos = body->GetWorldCenter();
	b2Vec2 vel = pos + s->getBody()->GetLinearVelocity();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(s->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();

	//Convert verts
	sf::Vector2f* verts = new sf::Vector2f[count];

	for (int i = 0; i < count; ++i)
	{
		
		verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
	}

	//Draw shape, vel, pos
	drawPolygon(verts, count, sf::Color(128, 0, 0));
	
	drawLine(B2toSF(pos, true), B2toSF(vel, true), sf::Color::Black);

	drawPoint(B2toSF(pos, true), sf::Color::White);
	
	//Clean up
	delete[] verts;
}

void GameDrawer::drawBounds(Bounds* const b)
{
	//Pull vars
	b2Body* body = b->getBody();
	b2Fixture* fix = body->GetFixtureList();
	
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

		drawPolygon(subVerts, count, sf::Color(64, 64, 64, l * 8), sf::Color(0, 0, 0));
	}


	//Draw stuff
	drawPolygon(verts, count, sf::Color(64, 64, 64, 128), sf::Color(0, 0, 0));

	//Clean up
	delete[] verts;
}

void GameDrawer::drawProjectile(Projectile* const p)
{
	//Pull vars
	b2Body* body = p->getBody();
	b2Vec2 pos = body->GetWorldCenter();

	b2Vec2 vel = pos + p->getBody()->GetLinearVelocity();
	b2CircleShape* shape = static_cast<b2CircleShape*>(p->getBody()->GetFixtureList()->GetShape());
	
	float rad = shape->m_radius * _SCALE_;

	//Draw shape, vel, pos
	drawCircle(B2toSF(pos, true), rad, sf::Color(196, 196, 0), sf::Color(128, 128, 0));
	drawLine(B2toSF(pos, true), B2toSF(vel, true), sf::Color::White);
	drawPoint(B2toSF(pos, true), sf::Color::Magenta);
}

void GameDrawer::drawSide(Side* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	b2Vec2 pos = s->getPosition();


	b2Fixture* fix = body->GetFixtureList();

	//It must be one of two
	if (fix->GetUserData() != "side")
	{
		fix = fix->GetNext();
	}

	//Pull other vars
	b2EdgeShape* shape = static_cast<b2EdgeShape*>(fix->GetShape());

	//Not long enough sides
	b2Vec2 a = body->GetWorldPoint(shape->m_vertex1);
	b2Vec2 b = body->GetWorldPoint(shape->m_vertex2);

	b2Vec2 vel = pos + s->getBody()->GetLinearVelocity();

	//Draw line
	drawLine(B2toSF(a, true), B2toSF(b, true), sf::Color::Red);
}

/*GameDrawer::DrawShape(const &Shape s)
{
	s.draw(this);
}
*/

