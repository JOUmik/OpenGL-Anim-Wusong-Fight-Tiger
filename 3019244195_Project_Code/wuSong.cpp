#include "wuSong.h"

WuSong::WuSong()
{
	Model* core = new Model();
	parts["core"] = core;

	Model* body = new Model();
	body->setNormalize(false);
	body->generateCube(Blue, glm::vec3(0.4, 0.6, 0.35));
	body->setParent(core);
	body->texture_path = "assets/WuSong/body.png";
	body->reflectFactor = 0.5;
	parts["body"] = body;

	Model* head = new Model();
	head->setNormalize(false);
	head->generateCube(Brown, glm::vec3(0.24, 0.24, 0.24));
	head->setParent(body);
	head->rotatePoint = glm::vec3(0, -0.12, 0);
	head->texture_path = "assets/WuSong/head.png";
	parts["head"] = head;

	Model* left_arm = new Model();
	left_arm->setNormalize(false);
	left_arm->generateCube(Green, glm::vec3(0.2, 0.4, 0.24));
	left_arm->setParent(body);
	left_arm->rotatePoint = glm::vec3(0, 0.2, 0);
	left_arm->texture_path = "assets/WuSong/arm.png";
	parts["left_arm"] = left_arm;

	Model* right_arm = new Model();
	right_arm->setNormalize(false);
	right_arm->generateCube(Green, glm::vec3(0.2, 0.4, 0.24));
	right_arm->setParent(body);
	right_arm->rotatePoint = glm::vec3(0, 0.2, 0);
	right_arm->texture_path = "assets/WuSong/arm.png";
	parts["right_arm"] = right_arm;

	Model* left_hand = new Model();
	left_hand->setNormalize(false);
	left_hand->generateCube(Red, glm::vec3(0.1, 0.3, 0.16));
	left_hand->setParent(left_arm);
	left_hand->rotatePoint = glm::vec3(0, 0.15, 0);
	left_hand->texture_path = "assets/WuSong/hand.png";
	left_hand->reflectFactor = 0.8;
	parts["left_hand"] = left_hand;

	Model* right_hand = new Model();
	right_hand->setNormalize(false);
	right_hand->generateCube(Red, glm::vec3(0.1, 0.3, 0.16));
	right_hand->setParent(right_arm);
	right_hand->rotatePoint = glm::vec3(0, 0.15, 0);
	right_hand->texture_path = "assets/WuSong/hand.png";
	right_hand->reflectFactor = 0.8;
	parts["right_hand"] = right_hand;

	Model* left_leg = new Model();
	left_leg->setNormalize(false);
	left_leg->generateCube(Cyan, glm::vec3(0.15, 0.4, 0.2));
	left_leg->setParent(body);
	left_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	left_leg->texture_path = "assets/WuSong/leg.png";
	left_leg->reflectFactor = 0.8;
	parts["left_leg"] = left_leg;

	Model* right_leg = new Model();
	right_leg->setNormalize(false);
	right_leg->generateCube(Cyan, glm::vec3(0.15, 0.4, 0.2));
	right_leg->setParent(body);
	right_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	right_leg->texture_path = "assets/WuSong/leg.png";
	right_leg->reflectFactor = 0.8;
	parts["right_leg"] = right_leg;

	Model* left_foot = new Model();
	left_foot->setNormalize(false);
	left_foot->generateCube(Yellow, glm::vec3(0.2, 0.3, 0.24));
	left_foot->setParent(left_leg);
	left_foot->rotatePoint = glm::vec3(0, 0.15, 0);
	left_foot->texture_path = "assets/WuSong/foot.png";
	parts["left_foot"] = left_foot;

	Model* right_foot = new Model();
	right_foot->setNormalize(false);
	right_foot->generateCube(Yellow, glm::vec3(0.2, 0.3, 0.24));
	right_foot->setParent(right_leg);
	right_foot->rotatePoint = glm::vec3(0, 0.15, 0);
	right_foot->texture_path = "assets/WuSong/foot.png";
	parts["right_foot"] = right_foot;

	Model* weapon = new Model();
	weapon->setNormalize(false);
	weapon->generateCube(Yellow, glm::vec3(0.05, 0.05, 1.0));
	weapon->setParent(right_hand);
	weapon->rotatePoint = glm::vec3(0, 0.05, 0);
	weapon->texture_path = "assets/WuSong/weapon.png";
	parts["weapon"] = weapon;

	//ÎäËÉµÄstay×´Ì¬
	motions["stay"]["body"] = WuSongMotion(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.6, 0.35));
	motions["stay"]["head"] = WuSongMotion(vec3(0.0, 0.3 + 0.12, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.2, 0.2, 0.2));
	motions["stay"]["left_arm"] = WuSongMotion(vec3(0.2 + 0.1, 0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["stay"]["right_arm"] = WuSongMotion(vec3(-0.2 - 0.1, 0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["stay"]["left_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["stay"]["right_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["stay"]["left_leg"] = WuSongMotion(vec3(-0.1, -0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["stay"]["right_leg"] = WuSongMotion(vec3(0.1, -0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["stay"]["left_foot"] = WuSongMotion(vec3(-0.01, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["stay"]["right_foot"] = WuSongMotion(vec3(0.01, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["stay"]["weapon"] = WuSongMotion(vec3(0.0, -0.09, 0.4), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));

	//ÎäËÉµÄrun1×´Ì¬
	motions["run1"]["body"] = WuSongMotion(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.6, 0.35));
	motions["run1"]["head"] = WuSongMotion(vec3(0.0, 0.3 + 0.12, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.2, 0.2, 0.2));
	motions["run1"]["left_arm"] = WuSongMotion(vec3(0.2 + 0.1, 0.3 - 0.2, 0.0), vec3(45.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["run1"]["right_arm"] = WuSongMotion(vec3(-0.2 - 0.1, 0.3 - 0.2, 0.0), vec3(-45.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["run1"]["left_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(-30.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["run1"]["right_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(-30.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["run1"]["left_leg"] = WuSongMotion(vec3(-0.1, -0.3 - 0.2, 0.0), vec3(-45.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["run1"]["right_leg"] = WuSongMotion(vec3(0.1, -0.3 - 0.2, 0.0), vec3(45.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["run1"]["left_foot"] = WuSongMotion(vec3(-0.01, -0.2 - 0.15, 0.0), vec3(-0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["run1"]["right_foot"] = WuSongMotion(vec3(0.01, -0.2 - 0.15, 0.0), vec3(30.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["run1"]["weapon"] = WuSongMotion(vec3(0.0, -0.09, 0.4), vec3(30.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));

	//ÎäËÉµÄrun2×´Ì¬
	motions["run2"]["body"] = WuSongMotion(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.6, 0.35));
	motions["run2"]["head"] = WuSongMotion(vec3(0.0, 0.3 + 0.12, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.2, 0.2, 0.2));
	motions["run2"]["left_arm"] = WuSongMotion(vec3(0.2 + 0.1, 0.3 - 0.2, 0.0), vec3(-45.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["run2"]["right_arm"] = WuSongMotion(vec3(-0.2 - 0.1, 0.3 - 0.2, 0.0), vec3(45.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["run2"]["left_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(-30.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["run2"]["right_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(-30.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["run2"]["left_leg"] = WuSongMotion(vec3(-0.1, -0.3 - 0.2, 0.0), vec3(45.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["run2"]["right_leg"] = WuSongMotion(vec3(0.1, -0.3 - 0.2, 0.0), vec3(-45.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["run2"]["left_foot"] = WuSongMotion(vec3(-0.01, -0.2 - 0.15, 0.0), vec3(30.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["run2"]["right_foot"] = WuSongMotion(vec3(0.01, -0.2 - 0.15, 0.0), vec3(-0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["run2"]["weapon"] = WuSongMotion(vec3(0.0, -0.09, 0.4), vec3(-30.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));

	//ÎäËÉµÄattack×´Ì¬
	motions["attack"]["body"] = WuSongMotion(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.6, 0.35));
	motions["attack"]["head"] = WuSongMotion(vec3(0.0, 0.3 + 0.12, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.2, 0.2, 0.2));
	motions["attack"]["left_arm"] = WuSongMotion(vec3(0.2 + 0.1, 0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["attack"]["right_arm"] = WuSongMotion(vec3(-0.2 - 0.1, 0.3 - 0.2, 0.0), vec3(-120.0, 0.0, 0.0), vec3(0.3, 0.4, 0.4));
	motions["attack"]["left_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["attack"]["right_hand"] = WuSongMotion(vec3(0, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.1, 0.3, 0.16));
	motions["attack"]["left_leg"] = WuSongMotion(vec3(-0.1, -0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["attack"]["right_leg"] = WuSongMotion(vec3(0.1, -0.3 - 0.2, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.4, 0.2));
	motions["attack"]["left_foot"] = WuSongMotion(vec3(-0.01, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["attack"]["right_foot"] = WuSongMotion(vec3(0.01, -0.2 - 0.15, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));
	motions["attack"]["weapon"] = WuSongMotion(vec3(0.0, -0.09, 0.4), vec3(0.0, 0.0, 0.0), vec3(0.4, 0.3, 0.4));

	setMotion(motions["stay"]);
	currentMotion = motions["stay"];
}

WuSong::~WuSong()
{
}

void WuSong::playMotion()
{
	if (timer <= 0)
	{
		if (!isInCycle) return;
		cycleIndex++;
		cycleIndex %= cycleList.size();
		startMotion(cycleList[cycleIndex], 1.0);
		return;
	}
	timer -= 0.05;
	for (auto it = currentMotion.begin(); it != currentMotion.end(); it++)
	{
		std::string pName = it->first;
		WuSongMotion& motion = it->second;
		GLfloat b = 0.05;
		motion.translate += b * (nextMotion[pName].translate - lastMotion[pName].translate);
		motion.rotate += b * (nextMotion[pName].rotate - lastMotion[pName].rotate);
	}

	setMotion(currentMotion);
}

void WuSong::setMotion(std::map<std::string, WuSongMotion>& m)
{
	for (auto it = m.begin(); it != m.end(); it++)
	{
		std::string pName = it->first;
		WuSongMotion motion = it->second;

		parts[pName]->setTranslation(motion.translate);
		parts[pName]->setRotation(motion.rotate);
		parts[pName]->setScale(motion.scale);
	}
}

void WuSong::startMotion(std::string next, float speed)
{
	lastMotion = currentMotion;
	nextMotion = motions[next];
	timer = speed;
}

void WuSong::startCycle(std::vector<std::string> list)
{
	cycleList = list;
	cycleIndex = 0;
	isInCycle = true;
	startMotion(cycleList[0], 1.0);
}

void WuSong::endCycle(std::string mName)
{
	isInCycle = false;
	startMotion(mName, 1.0);
}
