package clueGame;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.UnsupportedAudioFileException;

public class MusicPlayer {
	private static final int BUFFER_SIZE = 4096;
	private InputStream soundFile;
	
	MusicPlayer() {
		soundFile = this.getClass().getResourceAsStream("/data/BGM.wav");
	}
	
	  void play() {
	        try {
	          // converting the audio file to a stream
	          AudioInputStream sampleStream = AudioSystem.getAudioInputStream(new BufferedInputStream(soundFile));
	    
	          AudioFormat formatAudio = sampleStream.getFormat();
	    
	          DataLine.Info info = new DataLine.Info(SourceDataLine.class, formatAudio);
	    
	          SourceDataLine theAudioLine = (SourceDataLine) AudioSystem.getLine(info);
	    
	          theAudioLine.open(formatAudio);
	    
	          theAudioLine.start();
	    
	          System.out.println("Audio Player Started.");
	    
	          byte[] bufferBytes = new byte[BUFFER_SIZE];
	          int readBytes = -1;
	    
	          while ((readBytes = sampleStream.read(bufferBytes)) != -1) {
	            theAudioLine.write(bufferBytes, 0, readBytes);
	          }
	    
	          theAudioLine.drain();
	          theAudioLine.close();
	          sampleStream.close();
	    
	          System.out.println("Playback has been finished.");
	    
	        } catch (UnsupportedAudioFileException e) {
	          System.out.println("Unsupported file.");
	          e.printStackTrace();
	        } catch (LineUnavailableException e) {
	          System.out.println("Line not found.");
	          e.printStackTrace();
	        } catch (IOException e) {
	          System.out.println("Experienced an error.");
	          e.printStackTrace();
	        }
	      }
}
