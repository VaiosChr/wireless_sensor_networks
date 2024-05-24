import random

rooms = [0, 1, 2, 3, 4, 5, 6, 7, 8]
counts = [0, 0, 0, 0, 0, 0, 0, 0]

def main():
  with open("data/data.txt", "w") as file:
    for i in range(1000):
      room = find_room()
      update_counts(room)
      file.write(f"room = {room}, tag_id = {random.randint(0, 9)}\n")

# find a random room that is not empty
def find_room():
  if all(count == 0 for count in counts):
    return 0
  
  r = random.randint(0, 99)
  if r > 85:
    return 0
  else:
    not_empty_rooms = [room for room, count in zip(rooms, counts) if count > 0]
    return random.choice(not_empty_rooms) + 1
  
# update the counts list based on the room
def update_counts(room):
  if room == 8:
    counts[7] -= 1
    return
  
  counts[room] += 1
  if room > 0:
    counts[room-1] -= 1


if __name__ == "__main__":
  main()