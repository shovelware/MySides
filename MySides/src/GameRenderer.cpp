#include "GameRenderer.hpp"

sf::Vector2f GameRenderer::B2toSF(const b2Vec2& vec, bool scale) const
{
	return sf::Vector2f(vec.x * (scale ? _SCALE_ : 1.f), vec.y * (scale ? _SCALE_ : 1.f));
}

sf::Color GameRenderer::B2toSF(const b2Color& col) const
{
	return sf::Color(
		255U * col.r,
		255U * col.g,
		255U * col.b,
		255U * col.a);
}

sf::Color GameRenderer::tweakAlpha(const sf::Color & col, int alpha) const
{
	return sf::Color(col.r, col.g, col.b, alpha);
}

sf::Color GameRenderer::blend(const sf::Color& a, int aParts, const sf::Color& b, int bParts) const
{
	
	int totalParts = aParts + bParts;

	sf::Color total(
		(a.r * aParts + b.r * bParts) / totalParts,
		(a.g * aParts + b.g * bParts) / totalParts,
		(a.b * aParts + b.b * bParts) / totalParts);

		return total;
}

GameRenderer::GameRenderer(sf::RenderWindow& win, GameWorld* world) : Renderer(win), world_(world) 
{
}

void GameRenderer::render()
{
	drawBounds(world_->getBounds());

	std::list<Side*>& sides = world_->getSides();
	if (!sides.empty())
	{
		for (Side* sd : sides)
		{
			if (sd->getActive())
				drawSide(sd);
		}
	}
	
	std::list<Projectile*>& projs = world_->getProjectiles();
	if (!projs.empty())
	{
		for (Projectile* prj : projs)
		{
			if (prj->getActive())
				drawProjectile(prj);
		}
	}


	std::list<Enemy*>& shapes = world_->getShapes();
	if (!shapes.empty())
	{
		for (Enemy* shp : shapes)
		{
			if (shp->getActive())
				drawShape(shp);
		}
	}

	Shape* player = world_->getPlayer();
	if (player != nullptr)
	{
		if (player->getActive())
			drawShape(player);
	}

	std::list<Pickup::PickupI*>& pickups = world_->getPickups();
	
	if (!pickups.empty())
	{
		for (Pickup::PickupI* pic : pickups)
		{
			if (pic->getActive())
				drawPickup(pic);
		}
	}

	std::list<Force*>& forces = world_->getForces();
	if (!forces.empty())
	{
		for (Force* force : forces)
		{
			if (force->getActive())
				drawForce(force);
		}
	}

}

void GameRenderer::drawShape(Shape* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	b2PolygonShape* shape = static_cast<b2PolygonShape*>(s->getBody()->GetFixtureList()->GetShape());
	int count = shape->GetVertexCount();
	float size = s->getSize();
	float scaledSize = size * _SCALE_;
	float angle = body->GetAngle();

	int hp = s->getHP();
	int hpm = s->getHPMax();

	bool alive = s->getAlive();
	float spawnRemaining = (float)s->getSpawnTime() / (float)s->getSpawnTimeMax();

	sf::Vector2f pointing(sin(angle), -cos(angle));
	pointing *= (float)(_SCALE_ * s->getSize() * 1.25);
	
	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = B2toSF(body->GetLinearVelocity(), true);
	vel *= (float)(size * _SCALE_ * 2);
	
	sf::Color pri = B2toSF(s->getPrimary());
	sf::Color sec = B2toSF(s->getSecondary());
	sf::Color ter = B2toSF(s->getTertiary());

	//Spawning fade
	if (!alive)
	{
		pri.a *= (1.f - spawnRemaining);
		sec.a *= (1.f - spawnRemaining);
		ter.a *= (1.f - spawnRemaining);
		
	}

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

	//Orientation circle
	drawCircle(pos - pointing, size * 4, (armed ? ter : tweakAlpha(ter, 64)), (armed ? sec : tweakAlpha(sec, 64)), 0);
	if (alive) drawCircle(pos - (pointing *  2.f), size * 3, (armed ? tweakAlpha(sec, 128) : tweakAlpha(sec, 32)), (armed ? tweakAlpha(ter, 128) : tweakAlpha(ter, 32)), 0);
	if (alive) drawCircle(pos - (pointing *  3.f), size * 2, (armed ? tweakAlpha(ter, 196) : tweakAlpha(ter, 32)), (armed ? tweakAlpha(sec, 196) : tweakAlpha(sec, 32)), 0);
	
	//Actual shape
	drawPolygon(verts, count, pri, sec);

	//Centre circle
	drawCircle(pos, scaledSize / 8, sec, sec, 0);

	//Spawning shield
	if (!alive)
	{
		for (int i = 0; i < count; ++i)
		{
			sf::Vector2f newVert(pos);
			newVert = verts[i] - pos;
			newVert *= 1.2f;
			newVert += pos;
			verts[i] = newVert;
		}

		if (hp > 0)
		{
			ter.a = 28 + 100 * spawnRemaining;
			pri.a = 28 + 100 * spawnRemaining;


			drawPolygon(verts, count, ter, pri, (-scaledSize * 0.5f * spawnRemaining));
		}
	}

	//Clean up
	delete[] verts;
}

void GameRenderer::drawBounds(Bounds& const b)
{
	//Pull vars
	b2Body* body = b.getBody();
	b2Fixture* fix = body->GetFixtureList();
	
	sf::Vector2f pos = B2toSF(b.getPosition(), true);
	float radius = b.getRadius();

	sf::Color pri = B2toSF(b.getPrimary());
	sf::Color sec = B2toSF(b.getSecondary());
	sf::Color ter = B2toSF(b.getTertiary());
	
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

	//Copy of verts for inner circle drawing
	sf::Vector2f* subVerts = new sf::Vector2f[count];
	for (int i = 0; i < count; ++i)
	{
		subVerts[i] = verts[i];
	}
	int layers = 7;

	//Draw stuff
	//Draw base polygon
	drawPolygon(verts, count, sec, tweakAlpha(ter, 64), radius);

	//Draw other layers, getting smaller
	for (int l = 0; l <= layers - 2; ++l)
	{
		for (int v = 0; v < count; ++v)
		{
			subVerts[v] *= 0.9f;
		}

		drawPolygon(subVerts, count, (blend(pri, l + 1, sec, layers - 2)), tweakAlpha(ter, 48), -1);
	}

	//Web pattern
	for (int v = 0; v < count; ++v)
	{
		drawLine(pos, verts[v], tweakAlpha(ter, 48));
	}

	//Last polygon
	drawPolygon(subVerts, count, (blend(pri, layers, sec, layers - 2)), tweakAlpha(ter, 48), -1);

	//Clean up
	delete[] verts;
	delete[] subVerts;
}

void GameRenderer::drawProjectile(Projectile* const p)
{
	//Pull vars
	b2Body* body = p->getBody();

	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);
	sf::Vector2f vel = B2toSF(body->GetLinearVelocity(), true);
	bool alive = p->getAlive();
	float lifetime = p->getNormalisedLifeTime();
	float length = thor::length(vel);
	if (length > 4) 
	{
		vel = thor::unitVector(vel);
		vel *= fminf(length, (2));
	}

	vel *= (float)_SCALE_;
	
	//std::cout << thor::length(vel) << std::endl;

	sf::Color pri = B2toSF(p->getPrimary());
	sf::Color sec = B2toSF(p->getSecondary());
	sf::Color ter = B2toSF(p->getTertiary());

	pri.a *= (0.4f + (lifetime * 0.6f));
	sec.a *= (0.4f + (lifetime * 0.6f));
	ter.a *= (0.4f + (lifetime * 0.6f));

	b2Fixture* fixture = p->getBody()->GetFixtureList();
	if (fixture->GetUserData() == "projtracking")
	{
		fixture = fixture->GetNext();
	}

	if (b2CircleShape* shape = dynamic_cast<b2CircleShape*>(fixture->GetShape()))
	{
		float rad = shape->m_radius * _SCALE_;

		//Draw shape, vel, pos
		drawLine(pos, pos - vel, ter);
		drawCircle(pos, rad, pri, sec);
		drawPoint(pos, ter);
	}

	else if (b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape()))
	{
		int count = shape->GetVertexCount();

		sf::Vector2f* verts = new sf::Vector2f[count];

		for (int i = 0; i < count; ++i)
		{

			verts[i] = B2toSF(body->GetWorldPoint(shape->GetVertex(i)), true);
		}

		//drawLine(pos, pos - vel, ter);
		drawPolygon(verts, count, pri, sec);
		drawPoint(pos, ter);

		delete[] verts;
	}
}

void GameRenderer::drawSide(Side* const s)
{
	//Pull vars
	b2Body* body = s->getBody();
	float length = s->getValue();
	float lifeTimeNorm = s->getTimer();
	sf::Vector2f pos = B2toSF(s->getPosition(), true);
	sf::Vector2f dir = B2toSF(s->getHeading(), true);

	sf::Color pri = B2toSF(s->getPrimary());
	sf::Color sec = B2toSF(s->getSecondary());
	sf::Color ter = B2toSF(s->getTertiary());

	b2Fixture* fix = body->GetFixtureList();

	//It must be one of two
	if (fix->GetUserData() != "side") {	fix = fix->GetNext(); }

	//Pull other vars
	b2EdgeShape* shape = static_cast<b2EdgeShape*>(fix->GetShape());

	sf::Vector2f a = B2toSF(body->GetWorldPoint(shape->m_vertex1), true);
	sf::Vector2f b = B2toSF(body->GetWorldPoint(shape->m_vertex2), true);

	//Draw line
	drawLine(a, b, tweakAlpha(pri, 255 * lifeTimeNorm));
	drawCircle(pos, thor::length(dir), tweakAlpha(pri, 32 * lifeTimeNorm + .25f), tweakAlpha(sec, 64 * lifeTimeNorm + .5f));
	//drawCircle(pos, length, sec, pri, 0);
}

void GameRenderer::drawPickup(Pickup::PickupI * const p)
{
	if (p->getCollected() == false)
	{
		b2Body* body = p->getBody();

		sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);

		sf::Color pri = B2toSF(p->getPrimary());
		sf::Color sec = B2toSF(p->getSecondary());
		sf::Color ter = B2toSF(p->getTertiary());

		b2CircleShape* shape = static_cast<b2CircleShape*>(p->getBody()->GetFixtureList()->GetShape());
		float rad = shape->m_radius * _SCALE_;

		drawCircle(pos, rad, pri, sec);
		drawCircle(pos, rad / 2, sec, ter);
	}

	else
	{
		if (Pickup::Sight* s = dynamic_cast<Pickup::Sight*>(p))
		{
			b2Body* body = p->getBody();
			b2EdgeShape* ed = static_cast<b2EdgeShape*>(s->getBody()->GetFixtureList()->GetShape());
			sf::Vector2f end = B2toSF(s->getPosition(), true);
			sf::Vector2f beg = B2toSF(body->GetWorldPoint(ed->m_vertex2), true);
			sf::Vector2f mid = end - beg;
			bool contact = s->getContact();

			sf::Color pri = B2toSF(p->getPrimary());
			sf::Color sec = B2toSF(p->getSecondary());
			sf::Color ter = B2toSF(p->getTertiary());

			//drawLine(end, mid, pri);
			//drawLine(mid, end, sec);
			drawLine(end, beg, tweakAlpha((contact ? ter : sec), 32));
		}

		else if (Pickup::Shield* s = dynamic_cast<Pickup::Shield*>(p))
		{
			b2Body* body = p->getBody();

			sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);

			sf::Color pri = B2toSF(p->getPrimary());
			sf::Color sec = B2toSF(p->getSecondary());
			sf::Color ter = B2toSF(p->getTertiary());

			b2CircleShape* shape = static_cast<b2CircleShape*>(p->getBody()->GetFixtureList()->GetShape());
			float rad = shape->m_radius * _SCALE_;

			drawCircle(pos, rad, tweakAlpha(pri,128), tweakAlpha(sec, 128));
		}
	}

}

void GameRenderer::drawForce(Force* const f)
{
	//Pull vars
	b2Body* body = f->getBody();

	sf::Vector2f pos = B2toSF(body->GetWorldCenter(), true);

	float rad = f->getRadius() * _SCALE_;
	float force = f->getForce();
	bool in = (force < 0);
	
	float life = (f->getLifeTimePercent() / 100);

	sf::Color pri = B2toSF(f->getPrimary());
	sf::Color sec = B2toSF(f->getSecondary());
	sf::Color ter = B2toSF(f->getTertiary());

	//Indent scales based on lifetime in or out based on force
	float dent = (in ? -rad * (1.f - life) : (-rad) + (rad * (1.f -life)));
	//Draw shape
	drawCircle(pos, rad, tweakAlpha(blend(pri, 1, sec, 1), life * 32), tweakAlpha(ter, life * 48), dent);
}

/*GameRenderer::DrawShape(const &Shape s)
{
	s.draw(this);
}
*/

