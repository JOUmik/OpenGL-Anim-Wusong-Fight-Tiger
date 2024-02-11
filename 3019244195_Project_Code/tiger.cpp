#include "tiger.h"

Tiger::Tiger()
{
	Model* core = new Model();
	parts["core"] = core;

	Model* body = new Model();
	body->setNormalize(false);
	body->generateCube(Blue, glm::vec3(0.7, 0.6, 1.2));
	body->setParent(core);
	body->texture_path = "assets/tiger/body.png";
	parts["body"] = body;

	Model* head = new Model();
	head->setNormalize(false);
	head->generateCube(Brown, glm::vec3(0.45, 0.42, 0.4));
	head->setParent(body);
	head->rotatePoint = glm::vec3(0, 0, -0.2);
	head->texture_path = "assets/tiger/head.png";
	parts["head"] = head;

	Model* LeftUp_leg = new Model();
	LeftUp_leg->setNormalize(false);
	LeftUp_leg->generateCube(Green, glm::vec3(0.16, 0.6, 0.16));
	LeftUp_leg->setParent(body);
	LeftUp_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	LeftUp_leg->texture_path = "assets/tiger/rightLeg.png";
	parts["LeftUp_leg"] = LeftUp_leg;

	Model* LeftDown_leg = new Model();
	LeftDown_leg->setNormalize(false);
	LeftDown_leg->generateCube(Green, glm::vec3(0.16, 0.6, 0.16));
	LeftDown_leg->setParent(body);
	LeftDown_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	LeftDown_leg->texture_path = "assets/tiger/rightLeg.png";
	parts["LeftDown_leg"] = LeftDown_leg;

	Model* RightUp_leg = new Model();
	RightUp_leg->setNormalize(false);
	RightUp_leg->generateCube(Green, glm::vec3(0.16, 0.6, 0.16));
	RightUp_leg->setParent(body);
	RightUp_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	RightUp_leg->texture_path = "assets/tiger/leftLeg.png";
	parts["RightUp_leg"] = RightUp_leg;

	Model* RightDown_leg = new Model();
	RightDown_leg->setNormalize(false);
	RightDown_leg->generateCube(Green, glm::vec3(0.16, 0.6, 0.16));
	RightDown_leg->setParent(body);
	RightDown_leg->rotatePoint = glm::vec3(0, 0.2, 0);
	RightDown_leg->texture_path = "assets/tiger/leftLeg.png";
	parts["RightDown_leg"] = RightDown_leg;


	Model* tail = new Model();
	tail->setNormalize(false);
	tail->generateCube(Yellow, glm::vec3(0.16, 0.16, 0.6));
	tail->setParent(body);
	tail->rotatePoint = glm::vec3(0, 0, 0.25);
	tail->texture_path = "assets/tiger/tail.png";
	parts["tail"] = tail;

	motions["stay"]["body"] = TigerMotion(vec3(0.0, 0.9, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.7, 0.6, 1.2));
	motions["stay"]["head"] = TigerMotion(vec3(0.0, 0.1, 0.6 + 0.2), vec3(0.0, 0.0, 0.0), vec3(0.45, 0.42, 0.4));
	motions["stay"]["LeftUp_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, 0.4), vec3(0.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["stay"]["LeftDown_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, -0.4), vec3(0.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["stay"]["RightUp_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, 0.4), vec3(0.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["stay"]["RightDown_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, -0.4), vec3(0.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["stay"]["tail"] = TigerMotion(vec3(0.0, 0.0, -0.6 - 0.3), vec3(-30.0, 0.0, 0.0), vec3(0.16, 0.16, 0.6));

	motions["run1"]["body"] = TigerMotion(vec3(0.0, 0.9, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.7, 0.6, 1.2));
	motions["run1"]["head"] = TigerMotion(vec3(0.0, 0.1, 0.6 + 0.2), vec3(0.0, 0.0, 0.0), vec3(0.45, 0.42, 0.4));
	motions["run1"]["LeftUp_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, 0.4), vec3(45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run1"]["LeftDown_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, -0.4), vec3(45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run1"]["RightUp_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, 0.4), vec3(-45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run1"]["RightDown_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, -0.4), vec3(-45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run1"]["tail"] = TigerMotion(vec3(0.0, 0.0, -0.6 - 0.3), vec3(-30.0, 0.0, 0.0), vec3(0.16, 0.16, 0.6));

	motions["run2"]["body"] = TigerMotion(vec3(0.0, 0.9, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.7, 0.6, 1.2));
	motions["run2"]["head"] = TigerMotion(vec3(0.0, 0.1, 0.6 + 0.2), vec3(0.0, 0.0, 0.0), vec3(0.45, 0.42, 0.4));
	motions["run2"]["LeftUp_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, 0.4), vec3(-45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run2"]["LeftDown_leg"] = TigerMotion(vec3(-0.2, -0.4 - 0.2, -0.4), vec3(-45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run2"]["RightUp_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, 0.4), vec3(45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run2"]["RightDown_leg"] = TigerMotion(vec3(0.2, -0.4 - 0.2, -0.4), vec3(45.0, 0.0, 0.0), vec3(0.16, 0.6, 0.16));
	motions["run2"]["tail"] = TigerMotion(vec3(0.0, 0.0, -0.6 - 0.3), vec3(-30.0, 0.0, 0.0), vec3(0.16, 0.16, 0.6));

	setMotion(motions["stay"]);
	currentMotion = motions["stay"];
}

Tiger::~Tiger()
{
}

void Tiger::playMotion()
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
		TigerMotion& motion = it->second;
		GLfloat b = 0.05;
		motion.translate += b * (nextMotion[pName].translate - lastMotion[pName].translate);
		motion.rotate += b * (nextMotion[pName].rotate - lastMotion[pName].rotate);
	}

	setMotion(currentMotion);
}

void Tiger::setMotion(std::map<std::string, TigerMotion>& m)
{
	for (auto it = m.begin(); it != m.end(); it++)
	{
		std::string pName = it->first;
		TigerMotion motion = it->second;

		parts[pName]->setTranslation(motion.translate);
		parts[pName]->setRotation(motion.rotate);
		parts[pName]->setScale(motion.scale);
	}
}

void Tiger::startMotion(std::string next, float speed)
{
	lastMotion = currentMotion;
	nextMotion = motions[next];
	timer = speed;
}

void Tiger::startCycle(std::vector<std::string> list)
{
	//std::cout << "startCycle" << std::endl;
	cycleList = list;
	cycleIndex = 0;
	isInCycle = true;
	startMotion(cycleList[0], 1.0);
}

void Tiger::endCycle(std::string mName)
{
	//std::cout << "endCycle" << std::endl;
	isInCycle = false;
	startMotion(mName, 1.0);
}
