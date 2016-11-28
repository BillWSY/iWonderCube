import processing.sound.*;
import cc.arduino.*;
import org.firmata.*;
import processing.serial.*;

class MemorySide {

  SoundFile sound;
  long playEnd;
  boolean finished;

  final String[] AUDIO_FILENAMES = {
    "baby_laughing_1.mp3",  // 0
    "baby_laughing_2.mp3",  // 1
    "Door_Bell_1.mp3",      // 2
    "Door_Bell_2.mp3",      // 3
    "morning_1.mp3",        // 4
    "morning_2.mp3",        // 5
    "music_box_1.mp3",      // 6
    "music_box_2.mp3",      // 7
    "puppy_1.mp3",          // 8
    "puppy_2.mp3",          // 9
    "sea_wave_1.mp3",       // 10
    "sea_wave_2.mp3",       // 11
    "thunder_1.wav",        // 12
    "thunder_2.wav",        // 13
    "water_drop_1.mp3",     // 14
    "water_drop_2.mp3"      // 15
  };

  //                    0              5              10             15
  int[] AUDIO_LABELS = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8};

  boolean[] matched = new boolean[16];
  int matched_cnt = 0;

  int lastSoundLabel = -1;
  int lastkeyLabel = -1;

  public void startPlay(int soundLabel) {
    // If play has not ended, and current sound to play is different from the
    // last one, we need to stop playing first
    if (System.currentTimeMillis() < playEnd && lastSoundLabel != soundLabel) {
      sound.stop();
    }

    // If the last play has finished or the user requested to play a new sound,
    // we just play it and save its labels / expected end timestamp
    if (System.currentTimeMillis() > playEnd || lastSoundLabel != soundLabel) { 
      sound = new SoundFile(this, AUDIO_FILENAMES[soundLabel]);
      sound.play();
      lastSoundLabel = soundLabel;
      playEnd = System.currentTimeMillis() + (long)(sound.duration() * 1000);
    }  
  }

  void draw() {
    // Draw a 4x4 matrix, and differentiate those has been matched
    ellipse(56, 46, 55, 55);
  }

  void keypadDown(int keyLabel) {
    if (matched[keyLabel] == false) {
      startPlay(keyLabel);
      if (AUDIO_LABELS[keyLabelOriginal] == AUDIO_LABELS[keyLabel] 
          && keyLabelOriginal != keyLabel) {
        matched[keyLabel] = true;
        matched[keyLabelOriginal] = true;
        matched_cnt += 2;
        if (matched_cnt == 16) {
          finished = true;
          System.out.print("Congratulation!");
        }
      }
    }
  }

  // Getter for finishing status
  bool isFinished() {
    return finished;
  }
}
