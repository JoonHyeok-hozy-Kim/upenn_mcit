import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Scanner;

public class SocialNetworkRunner {
    HashMap<String, ArrayList<String>> relationMap = new HashMap<>();
    
    public String mostFollowers() {
        String max_name = "";
        int max_followers = 0;

        for (String curr_name: relationMap.keySet()){
            int curr_followers = relationMap.get(curr_name).size();
            if (max_name == "" || curr_followers > max_followers){
                max_name = curr_name;
                max_followers = curr_followers;
            }
        }
        
        return max_name;
    }

    public int followersInCommon(String personOne, String personTwo) {
        if (!relationMap.containsKey(personOne) || !relationMap.containsKey(personTwo)){
            return 0;
        }

        int i = 0;
        int j = 0;
        int count = 0;

        ArrayList<String> container1 = relationMap.get(personOne);
        ArrayList<String> container2 = relationMap.get(personTwo);

        Collections.sort(container1);
        Collections.sort(container2);

        while (i < container1.size() && j < container2.size()){
            System.out.println(i + " " + j);
            if (container1.get(i).equals(container2.get(j))){
                count++;
                i++;
                j++;
            } else if (container1.get(i).compareTo(container2.get(j)) > 0){
                j++;
            } else{
                i++;
            }
        }
        return count;
    }

    public void scanFile(Scanner scanner){
        while (scanner.hasNext()){
            String str = scanner.nextLine();
            parseRelations(str);
        }

        System.out.println(relationMap);
    }

    public void parseRelations(String str){
        String[] splitStr = str.split(":");
        String followerName = splitStr[0].trim();
        ArrayList<String> followings = new ArrayList<>();
        for (String f: splitStr[1].split(",")){
            String curr_followed = f.trim();
            if (!curr_followed.equals("")){
                if (!relationMap.containsKey(curr_followed)){
                    ArrayList<String> followers = new ArrayList<>();
                    relationMap.put(curr_followed, followers);
                }
                relationMap.get(curr_followed).add(followerName);
            }            
        }
    }
}