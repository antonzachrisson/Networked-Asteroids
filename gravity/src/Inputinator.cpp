#include "Inputinator.hpp"


//Version 1

void Inputinator::CheckOffset(server_info_message msg, Game* game)
{
	for (int i = 0; i < m_predictions.size(); i++)
	{
		if (m_predictions[i].m_tick == msg.m_acknowledge)
		{
			bool clearPredictionsRotation = false;
			bool clearPredictionsPosition = false;

			sf::Vector2f serverPos;
			serverPos.x = msg.m_playerPos[0];
			serverPos.y = msg.m_playerPos[1];
			sf::Vector2f delta = m_predictions[i].m_position - serverPos;
			float displacement = sqrtf(delta.x * delta.x + delta.y * delta.y);

			float rotOffset = abs(m_predictions[i].m_rotation - msg.m_playerPos[2]);
			if (rotOffset > 5.0f)
			{
				m_predictions[i].m_rotation = msg.m_playerPos[2];

				inputPrediction m_input;

				for (int z = i; z < m_predictions.size() - 1; z++)
				{
					for (int u = 0; u < m_inputPredictions.size() - 1; u++)
					{
						if (m_inputPredictions[u].m_tick == m_predictions[z].m_tick)
						{
							m_input = m_inputPredictions[u];
						}
					}
					if (m_input.m_left)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation - (simRate * kPlayerRotationSpeed);
					}
					if (m_input.m_right)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation + (simRate * kPlayerRotationSpeed);
					}
				}

				game->playerptr.rotation = m_predictions[m_predictions.size() - 1].m_rotation;
				game->playerptr.Sprite.setRotation(game->playerptr.rotation);

				clearPredictionsRotation = true;
			}

			if (displacement > 5.0f)
			{
				m_predictions[i].m_position.x = msg.m_playerPos[0];
				m_predictions[i].m_position.y = msg.m_playerPos[1];

				inputPrediction m_input;

				for (int z = i; z < m_predictions.size() - 1; z++)
				{
					for (int u = 0; u < m_inputPredictions.size() - 1; u++)
					{
						if (m_inputPredictions[u].m_tick == m_predictions[z].m_tick)
						{
							m_input = m_inputPredictions[u];
						}
					}

					sf::Vector2f velocity;

					if (m_input.m_up == false)
					{
						velocity.x = 0.0f;
						velocity.y = 0.0f;
					}

					else
					{
						float temprot = m_predictions[z].m_rotation - 90.0f;
						velocity.x = cosf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
						velocity.y = sinf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
					}

					float deltaSpeed = simRate * kPlayerMoveSpeed;
					sf::Vector2f move;
					move = velocity * deltaSpeed;

					if (m_input.m_left)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation - (simRate * kPlayerRotationSpeed);
					}
					if (m_input.m_right)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation + (simRate * kPlayerRotationSpeed);
					}

					m_predictions[z + 1].m_position = m_predictions[z].m_position + move;
				}

				game->playerptr.position = m_predictions[m_predictions.size() - 1].m_position;
				game->playerptr.Sprite.setPosition(game->playerptr.position);

				game->playerptr.rotation = m_predictions[m_predictions.size() - 1].m_rotation;
				game->playerptr.Sprite.setRotation(game->playerptr.rotation);

				clearPredictionsPosition = true;
			}

			if (clearPredictionsRotation || clearPredictionsPosition)
			{
				for (int o = 0; o < m_predictions.size() - 1; o++)
				{
					m_predictions.erase(m_predictions.begin());
				}
			}
		}
	}
}

void Inputinator::addPrediction(uint32 tick, sf::Vector2f position, bool up, bool left, bool right, float rot)
{
	m_predictions.push_back(prediction(tick, position, up, left, right, rot));
}

void Inputinator::addInputPrediction(uint32 tick, bool up, bool left, bool right)
{
	m_inputPredictions.push_back(inputPrediction(tick, up, left, right));
}


//Version 2

/*
void Inputinator::CheckOffset(server_info_message msg, Game* game)
{
	for (int i = 0; i < m_predictions.size(); i++)
	{
		if (m_predictions[i].m_tick == msg.m_acknowledge)
		{
			sf::Vector2f serverPos;
			serverPos.x = msg.m_playerPos[0];
			serverPos.y = msg.m_playerPos[1];
			sf::Vector2f delta = m_predictions[i].m_position - serverPos;
			float displacement = sqrtf(delta.x * delta.x + delta.y * delta.y);

			if (displacement > 5.0f)
			{
				m_predictions[i].m_position.x = msg.m_playerPos[0];
				m_predictions[i].m_position.y = msg.m_playerPos[1];
				m_predictions[i].m_rotation = msg.m_playerPos[2];

				for (int y = 0; y < i; y++)
				{
					m_predictions.erase(m_predictions.begin());
				}

				for (int z = 0; z < m_predictions.size() - 1; z++)
				{
					sf::Vector2f velocity;

					if (m_predictions[z].m_up == false)
					{
						velocity.x = 0.0f;
						velocity.y = 0.0f;
					}

					else
					{
						float temprot = m_predictions[z].m_rotation - 90.0f;
						velocity.x = cosf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
						velocity.y = sinf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
					}

					float deltaSpeed = simRate * kPlayerMoveSpeed;
					sf::Vector2f move = velocity * deltaSpeed;

					m_predictions[z + 1].m_position = m_predictions[z].m_position + move;

					if (m_predictions[z].m_left)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation - (simRate * kPlayerRotationSpeed);
					}
					if (m_predictions[z].m_right)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation + (simRate * kPlayerRotationSpeed);
					}
				}

				for (int o = 0; o < m_predictions.size() - 1; o++)
				{
					m_predictions.erase(m_predictions.begin());
				}


				game->playerptr.position = m_predictions[m_predictions.size() - 1].m_position;
				game->playerptr.Sprite.setPosition(game->playerptr.position);

				game->playerptr.rotation = m_predictions[m_predictions.size() - 1].m_rotation;
				game->playerptr.Sprite.setRotation(game->playerptr.rotation);
			}
		}
	}
}
*/


//Version 3

/*
void Inputinator::CheckOffset(server_info_message msg, Game* game)
{
	for (int i = 0; i < m_predictions.size(); i++)
	{
		if (m_predictions[i].m_tick == msg.m_acknowledge)
		{
			bool clearPredictionsRotation = false;
			bool clearPredictionsPosition = false;
			
			sf::Vector2f serverPos;
			serverPos.x = msg.m_playerPos[0];
			serverPos.y = msg.m_playerPos[1];
			sf::Vector2f delta = m_predictions[i].m_position - serverPos;
			float displacement = sqrtf(delta.x * delta.x + delta.y * delta.y);
			//temp
			std::cout << displacement << std::endl;

			float rotOffset = abs(m_predictions[i].m_rotation - msg.m_playerPos[2]);
			if (rotOffset > 5.0f)
			{
				m_predictions[i].m_rotation = msg.m_playerPos[2];

				for (int z = i; z < m_predictions.size() - 1; z++)
				{
					if (m_predictions[z].m_left)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation - (simRate * kPlayerRotationSpeed);
					}
					if (m_predictions[z].m_right)
					{
						m_predictions[z + 1].m_rotation = m_predictions[z].m_rotation + (simRate * kPlayerRotationSpeed);
					}
				}

				game->playerptr.rotation = m_predictions[m_predictions.size() - 1].m_rotation;
				game->playerptr.Sprite.setRotation(game->playerptr.rotation);

				clearPredictionsRotation = true;
			}

			if (displacement > 5.0f)
			{
 				m_predictions[i].m_position.x = msg.m_playerPos[0];
				m_predictions[i].m_position.y = msg.m_playerPos[1];

				for (int z = i; z < m_predictions.size() - 1; z++)
				{
					sf::Vector2f velocity;
					
					if (m_predictions[z].m_up == false)
					{
						velocity.x = 0.0f;
						velocity.y = 0.0f;
					}

					else
					{
						float temprot = m_predictions[z].m_rotation - 90.0f;
						velocity.x = cosf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
						velocity.y = sinf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
					}

					float deltaSpeed = simRate * kPlayerMoveSpeed;
					sf::Vector2f move;
					move = velocity * deltaSpeed;

					m_predictions[z + 1].m_position = m_predictions[z].m_position + move;
				}

				game->playerptr.position = m_predictions[m_predictions.size() - 1].m_position;
				game->playerptr.Sprite.setPosition(game->playerptr.position);

				clearPredictionsPosition = true;
			}

			if (clearPredictionsRotation || clearPredictionsPosition)
			{
				for (int o = 0; o < m_predictions.size() - 1; o++)
				{
					m_predictions.erase(m_predictions.begin());
				}
			}
		}
	}
}*/



