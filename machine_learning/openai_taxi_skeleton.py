# Load OpenAI Gym and other necessary packages
import gym
import numpy as np
import random
import numpy
import time

# Environment
env = gym.make("Taxi-v3")

# Training parameters for Q learning
alpha = 0.9 # Learning rate
gamma = 0.9 # Future reward discount factor
num_of_episodes = 1000
num_of_steps = 500 # per each episode

# Q tables for rewards
#Q_reward = -100000*numpy.ones((500,6)) # All same
Q_reward = numpy.random.random((500, 6)) # Random

# Training w/ random sampling of actions
# YOU WRITE YOUR CODE HERE

for episode in range (num_of_episodes):
    tot_reward = 0
    reward = None
    done = False
    state = env.reset()

    for step in range (num_of_steps):

        action = env.action_space.sample()
        next_state, reward, done, info = env.step(action)
        Q_reward[state, action] += alpha * (reward + gamma*np.max(Q_reward[next_state]) - Q_reward[state, action])

        tot_reward += reward
        state = next_state

        if done:
            break


# Running the test 10 times, and calculating the average total reward and steps (actions) taken
rewards = 0
steps = 0

for i in range(10):

    state = env.reset()
    tot_reward = 0
    for t in range(50):
        steps += 1
        action = numpy.argmax(Q_reward[state,:])
        state, reward, done, info = env.step(action)
        tot_reward += reward
        env.render()
        time.sleep(1)
        if done:
            print("Total reward %d" %tot_reward)
            rewards += tot_reward
            break

print ("The average of 10 runs: Reward: {} Steps: {}".format(rewards/10, steps/10))