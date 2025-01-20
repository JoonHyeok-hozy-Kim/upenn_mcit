import org.junit.Test;
import static org.junit.Assert.*;

import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.util.*;

public class DocumentsProcessorTest {
    String localHome = "";

    String nanoDir = localHome + "nano_doc_set";
    String onlyOneTxt = localHome + "one_doc_set/only_one.txt";
    String microDir = localHome + "micro_doc_set";
    String emptyDir = localHome + "empty_doc_set";


    String nwordFilePath = localHome + "nwordFiles/nword.txt";


    DocumentsProcessor d = new DocumentsProcessor();

    @Test
    public void readFilesInDirectoryTest() {    
        Map<String, List<String>> fileMap = d.processDocuments(nanoDir, 2);
        Set<String> fileNames = fileMap.keySet();
        assertTrue(fileNames.contains("nano1.txt"));
        assertTrue(fileNames.contains("nano2.txt"));
    }

    @Test 
    public void generateNGramTest() {
        Map<String, List<String>> fileMapN2 = d.processDocuments(nanoDir, 2);
        List<String> nano1 = fileMapN2.get("nano1.txt");
        assertEquals("ab", nano1.get(0));
        assertEquals("bc", nano1.get(1));
        assertEquals("cd", nano1.get(2));

        Map<String, List<String>> fileMapN3 = d.processDocuments(nanoDir, 3);
        List<String> nano2 = fileMapN3.get("nano2.txt");
        assertEquals("xyz", nano2.get(0));
        assertEquals("yzw", nano2.get(1));
    }

    @Test
    public void storeNGramsDirectoryFileGenerationTest() {
        Map<String, List<String>> fileMapN3 = d.processDocuments(nanoDir, 3);
        d.storeNGrams(fileMapN3, nwordFilePath);

        File targetFile = new File(nwordFilePath);
        assertTrue(targetFile.exists());
    }
    
    @Test
    public void storeNGramsTest() {
        Map<String, List<String>> fileMapN3 = d.processDocuments(nanoDir, 3);
        List<Tuple<String, Integer>> summary = d.storeNGrams(fileMapN3, nwordFilePath);
        
        Tuple<String, Integer> t1 = new Tuple<>("nano1.txt", "abc bcd ".getBytes().length);
        Tuple<String, Integer> t2 = new Tuple<>("nano2.txt", "xyz yzw ".getBytes().length);
        boolean contained1 = false, contained2 = false;

        for (Tuple<String, Integer> t : summary) {
            // System.out.println(t.getLeft() + ", " + t.getRight());
            if (t.getLeft().equals(t1.getLeft()) && Objects.equals(t.getRight(), t1.getRight())) {
                contained1 = true;
                continue;
            }
            if (t.getLeft().equals(t2.getLeft()) && Objects.equals(t.getRight(), t2.getRight())) {
                contained2 = true;
            }
        }
        
        assertTrue(contained1 && contained2);
    }

    @Test
    public void computeSimilaritiesNumberOfSimsTest() {
        Map<String, List<String>> fileMapN3 = d.processDocuments(nanoDir, 3);
        List<Tuple<String, Integer>> summary = d.storeNGrams(fileMapN3, nwordFilePath);
        TreeSet<Similarities> ts = d.computeSimilarities(nwordFilePath, summary);
        assertEquals(2, ts.size());
    }

    @Test
    public void createPartialReaderTest() throws IOException {
        File oneDoc = new File(onlyOneTxt);
        try (Reader oneRead = DocumentsProcessor.createPartialReader(oneDoc, 1, 19)) {
            DocumentIterator di = new DocumentIterator(oneRead, 1);
            assertEquals("oneliness", di.next());   // Missing the beginning 'l'
            assertEquals("is", di.next());
            assertEquals("my", di.next());
            assertEquals("onl", di.next());     // Missing the last 'y'
            assertFalse(di.hasNext());
        }        
    }

    @Test
    public void computeSimilaritiesNanoDocTest() {
        Map<String, List<String>> fileMapNano = d.processDocuments(nanoDir, 3);
        List<Tuple<String, Integer>> summary = d.storeNGrams(fileMapNano, nwordFilePath);
        TreeSet<Similarities> ts = d.computeSimilarities(nwordFilePath, summary);
        for (Similarities s : ts) {
            System.out.println(s.toString());
        }
        assertEquals(2, ts.size());
    }

    @Test
    public void computeSimilaritiesMicroDocTest() {
        Map<String, List<String>> fileMapMicro = d.processDocuments(microDir, 3);
        List<Tuple<String, Integer>> summary = d.storeNGrams(fileMapMicro, nwordFilePath);
        TreeSet<Similarities> ts = d.computeSimilarities(nwordFilePath, summary);
        for (Similarities s : ts) {
            System.out.println(s.toString());
        }
        assertEquals(3, ts.size());
    }

    @Test
    public void printSimilaritiesMicroTest() {
        Map<String, List<String>> fileMapMicro = d.processDocuments(microDir, 3);
        List<Tuple<String, Integer>> summary = d.storeNGrams(fileMapMicro, nwordFilePath);
        TreeSet<Similarities> ts = d.computeSimilarities(nwordFilePath, summary);
        d.printSimilarities(ts, 2);
        assertTrue(ts.isEmpty());
    }

    @Test
    public void processAndStoreNanoTest() {
        List<Tuple<String, Integer>> summary = d.processAndStore(nanoDir, nwordFilePath, 3);
        
        Tuple<String, Integer> t1 = new Tuple<>("nano1.txt", "abc bcd ".getBytes().length);
        Tuple<String, Integer> t2 = new Tuple<>("nano2.txt", "xyz yzw ".getBytes().length);
        boolean contained1 = false, contained2 = false;

        for (Tuple<String, Integer> t : summary) {
            System.out.println(t.getLeft() + ", " + t.getRight());
            if (t.getLeft().equals(t1.getLeft()) && Objects.equals(t.getRight(), t1.getRight())) {
                contained1 = true;
                continue;
            }
            if (t.getLeft().equals(t2.getLeft()) && Objects.equals(t.getRight(), t2.getRight())) {
                contained2 = true;
            }
        }
        
        assertTrue(contained1 && contained2);
    }

    @Test
    public void printSimilaritiesEmptyTest() {
        List<Tuple<String, Integer>> summary = d.processAndStore(emptyDir, nwordFilePath, 5);
        TreeSet<Similarities> ts = d.computeSimilarities(nwordFilePath, summary);
        d.printSimilarities(ts, 1000);
        assertTrue(ts.isEmpty());
    }

    @Test
    public void fileIndexWithBinarySearchTest() {
        List<Integer> l = Arrays.asList(10, 20, 30, 40);
        assertEquals(-1, DocumentsProcessor.fileIndexWithBinarySearch(l, -5));
        assertEquals(-1, DocumentsProcessor.fileIndexWithBinarySearch(l, 41));
        assertEquals(0, DocumentsProcessor.fileIndexWithBinarySearch(l, 0));
        assertEquals(0, DocumentsProcessor.fileIndexWithBinarySearch(l, 10));
        assertEquals(1, DocumentsProcessor.fileIndexWithBinarySearch(l, 15));
        assertEquals(2, DocumentsProcessor.fileIndexWithBinarySearch(l, 27));
        assertEquals(3, DocumentsProcessor.fileIndexWithBinarySearch(l, 40));
    }
}
