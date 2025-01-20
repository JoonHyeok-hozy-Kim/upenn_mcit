import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;

public class DocumentsProcessor implements IDocumentsProcessor {
    @Override
    public Map<String, List<String>> processDocuments(String directoryPath, int n) {
        File dir = new File(directoryPath);
        File[] files = dir.listFiles();
        Map<String, List<String>> resultMap = new HashMap<>();

        // Case when not able to access directoryPath or no file exists in it.
        if (files == null) {
            System.err.println("Directory is invalid or inaccessible: " + directoryPath);
            return resultMap; // 혹은 null 등을 반환할 수도 있음
        }

        for (File f : files) {
            if (f.isFile()) {
                try (Reader r = new FileReader(f)) {
                    resultMap.put(f.getName(), new ArrayList<>());
                    DocumentIterator di = new DocumentIterator(r, n);
                    while (di.hasNext()) {
                        resultMap.get(f.getName()).add(di.next());
                    }
                } catch (IOException e) {
                    System.err.println("Encountered error while reading : " + f.getName());
//                    e.printStackTrace();
                }
            }
        }

        return resultMap;
    }

    @Override
    public List<Tuple<String, Integer>> storeNGrams(Map<String, List<String>> docs, 
                                                    String nwordFilePath) {
        List<Tuple<String, Integer>> resultList = new ArrayList<>();

        File nGramFile = new File(nwordFilePath);
        if (checkDir(nGramFile)) {
            return resultList;
        }

        for (String key: docs.keySet()) {
            int byteCnt = 0;
            for (String nGram : docs.get(key)) {
                try (FileWriter w = new FileWriter(nGramFile, true)) {
                    nGram += " ";
                    w.write(nGram);
                    byteCnt += nGram.getBytes().length;
                } catch (IOException e) {
                    System.err.println("Error occurred during writing on nGramTxt " + 
                                        e.getMessage());
                }
            }
            resultList.add(new Tuple<>(key, byteCnt));
        }
        return resultList;
    }

    private boolean checkDir(File nGramFile) {
        File dir = nGramFile.getParentFile();
        if (dir != null && !dir.exists()) {
            // Create the parent directory if not exists.
            dir.mkdirs();      
        }
        if (nGramFile.exists()) {
            // Delete the existing one to prevent appending on it.
            nGramFile.delete(); 
        }

        try {
            // Always create new file!
            nGramFile.createNewFile();  
        } catch (Exception e) {
            System.err.println("Error creating new file : " + e.getMessage());
//            e.printStackTrace();
            return true;
        }
        return false;
    }

    public List<Tuple<String, Integer>> processAndStore(String directoryPath, 
                                                String sequenceFile, int n) {     
        // System.out.println("[Job Notice] processAndStore started.");   

        List<Tuple<String, Integer>> resultList = new ArrayList<>();
        File dir = new File(directoryPath);
        File[] files = dir.listFiles();

        File nGramFile = new File(sequenceFile);
        if (checkDir(nGramFile)) {
            return resultList;
        }

        int byteCnt;
        String nGram;
        assert files != null;
        for (File f : files) {
            if (f.isFile()) {
                byteCnt = 0;
                try {
                    Reader r = new FileReader(f);
                    DocumentIterator di = new DocumentIterator(r, n);
                    FileWriter w = new FileWriter(nGramFile, true);
                    while (di.hasNext()) {
                        nGram = di.next();
                        nGram += " ";
                        w.write(nGram);
                        // System.out.println("nGram : " + nGram);
                        byteCnt += nGram.getBytes().length;
                    }
                    w.close();
                    r.close();
                    resultList.add(new Tuple<>(f.getName(), byteCnt));

                } catch (IOException e) {
                    System.err.println("Error occurred during writing on nGramTxt " + 
                                    e.getMessage());
                }

            }
        }

        return resultList;
    }

    @Override
    public TreeSet<Similarities> computeSimilarities(String nwordFilePath, 
                                        List<Tuple<String, Integer>> fileindex) {
        TreeSet<Similarities> resultTreeSet = new TreeSet<>();

        // Initialize every possible Similarities and add to simList
        List<List<Similarities>> simList = new ArrayList<>();
        for (int i = 0; i < fileindex.size(); i++) {
            ArrayList<Similarities> iRow = new ArrayList<>();
            for (int j = i + 1; j < fileindex.size(); j++) {
                iRow.add(new Similarities(fileindex.get(i).getLeft(), fileindex.get(j).getLeft()));
            }
            simList.add(iRow);
        }

        // Create a cumulative byte index array for fileIndexWithBinarySearch.
        ArrayList<Integer> cumByteIdxList = new ArrayList<>();
        int byteCum = 0;
        for (Tuple<String, Integer> t: fileindex) {
            byteCum += t.getRight();
            cumByteIdxList.add(byteCum);
        }

        // Open nwordsFile and read it words by words using DocumentIterator
        File nwordsFile = new File(nwordFilePath);
        Reader r;
        DocumentIterator di;
        try {
            r = new FileReader(nwordsFile);
            di = new DocumentIterator(r, 1);   // Read nwordsFile words by words.
        } catch (IOException e) {
            System.err.println("Error opening nwordsFile : " + e.getMessage());
//            e.printStackTrace();
            return resultTreeSet;
        }
        
        // Create a HashMap of nwords and fill them while iterating through the nwordsFile
        HashMap<String, List<Integer>> nwordMap = new HashMap<>();
        int byteSum = 0;
        Similarities sim;
        while (di.hasNext()) {
            String currNword = di.next();
            byteSum += currNword.length() + 1;

            // Find the index of the file that currNword is located at.
            int currIdx = fileIndexWithBinarySearch(cumByteIdxList, byteSum);

            if (!nwordMap.containsKey(currNword)) {
                /** If currNword does not exist in the HashMap, 
                 * add it as a key and its corresponding ArrayList. */
                nwordMap.put(currNword, new ArrayList<>());
            } else {
                /** If currNword exists in the HashMap, 
                 *  update Similarities with existing idx's. */
                for (int prevIdx: nwordMap.get(currNword)) {
                    if (prevIdx == currIdx) {
                        continue;
                    }
                    sim = simList.get(prevIdx).get(currIdx - prevIdx - 1);
                    sim.setCount(sim.getCount() + 1);
                }                
            }
            // Add currIdx to the corresponding ArrayList for later updates.
            nwordMap.get(currNword).add(currIdx);   
        }
        
        try {
            r.close();
        } catch (IOException e) {
            System.err.println("Error closing nwordsFile : " + e.getMessage());
//            e.printStackTrace();
            return resultTreeSet;
        }
        
        // Add all similarities to the TreeSet.
        for (List<Similarities> l : simList) {
            for (Similarities s : l) {
                if (s.getCount() > 0) {
                    resultTreeSet.add(s);
                }
            }
        }
        
        return resultTreeSet;
    }

    public static int fileIndexWithBinarySearch(List<Integer> byteCumList, int byteVal) {
        if (byteVal < 0 || byteVal > byteCumList.get(byteCumList.size() - 1)) {
            return -1;
        }

        int idx = Collections.binarySearch(byteCumList, byteVal);

        // Case that byteVal is not the element of byteCumList. i.e. in range!
        if (idx < 0) {
            idx = (idx + 1) * (-1);     // Reverse calculate the insertion point!
        }

        return idx;
    }
    
    public static Reader createPartialReader(File file, int start, int length) throws IOException {
        try (RandomAccessFile ra = new RandomAccessFile(file, "r")) {
            ra.seek(start);
            byte[] buffer = new byte[(int) length];
            ra.readFully(buffer);

            return new InputStreamReader(new ByteArrayInputStream(buffer));
        }
    }

    @Override        
    public void printSimilarities(TreeSet<Similarities> sims, int threshold) {
        while (!sims.isEmpty()) {
            Similarities s = sims.pollLast();
            assert s != null;
            if (s.getCount() < threshold) {
                break;
            }
            System.out.println(s);
        }
    }
}
