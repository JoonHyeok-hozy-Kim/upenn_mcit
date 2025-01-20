import static org.junit.Assert.*;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.Scanner;

import org.junit.Test;

public class SocialNetworkRunnerTest{
    @Test
    public void givenExampleTest1() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("example.txt"));
        SNR.scanFile(s);

        assertEquals(SNR.mostFollowers(), "Dana");
        assertEquals(SNR.followersInCommon("Dana", "Adrian"), 1);
    }

    @Test
    public void hozyExampleTest1() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("hozy_example1.txt"));
        SNR.scanFile(s);

        ArrayList<String> nameList = new ArrayList<String>(Arrays.asList("a", "b", "c"));
        assertEquals(SNR.mostFollowers(), "");
        assertEquals(SNR.followersInCommon("a", "b"), 0);
    }

    @Test
    public void hozyExampleTest2() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("hozy_example2.txt"));
        SNR.scanFile(s);

        ArrayList<String> nameList = new ArrayList<String>(Arrays.asList("a", "b", "c", "d"));
        assertTrue(nameList.contains(SNR.mostFollowers()));
        assertEquals(SNR.followersInCommon("a", "b"), 4-2);
    }

    @Test
    public void hozyExampleTest3() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("hozy_example3.txt"));
        SNR.scanFile(s);

        ArrayList<String> nameList = new ArrayList<String>(Arrays.asList("a", "b", "c", "d"));
        assertTrue(nameList.contains(SNR.mostFollowers()));
        assertEquals(SNR.followersInCommon("a", "b"), 2);
    }

    @Test
    public void hozyExampleTest4() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("hozy_example4.txt"));
        SNR.scanFile(s);

        assertEquals(SNR.followersInCommon("a", "b"), 0);
        assertEquals(SNR.followersInCommon("b", "c"), 0);
        assertEquals(SNR.followersInCommon("c", "a"), 0);        
    }

    @Test
    public void hozyExampleTest5() throws IOException{
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Scanner s = new Scanner(new File("hozy_example5.txt"));
        SNR.scanFile(s);

        assertEquals(SNR.mostFollowers(), "");    
    }

    @Test
    public void hozyExampleTest6(){
        SocialNetworkRunner SNR = new SocialNetworkRunner();
        Random r = new Random();

        // Generate random big number of people between 10 and 20 with their indices as their names.
        int numberOfPeople = r.nextInt(1000, 2000);
        for (int i=0; i<numberOfPeople; i++){

            // Use the index as the name.
            String temp_line = Integer.toString(i);
            temp_line += ": ";

            // Add followings.
            for (int j=0; j<i; j++){
                // Use numbers except itself as the followings.
                temp_line += Integer.toString(j);
                if (j == i-1) break;
                temp_line += ", ";
            }

            SNR.parseRelations(temp_line);
        }

        assertEquals(SNR.mostFollowers(), Integer.toString(0));  // Everyone follows "0".
        assertEquals(SNR.followersInCommon("0", "1"), numberOfPeople-2); // The mostly followed two "0" and "1" will have (numberOfPeople-2) number of followers.
    }
}
