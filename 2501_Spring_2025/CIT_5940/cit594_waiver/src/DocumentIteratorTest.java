import org.junit.Test;
import static org.junit.Assert.*;

import java.io.Reader;
import java.io.StringReader;

public class DocumentIteratorTest {
    private final String simpleText = "This is REALLY simple.";

    @Test
    public void nextConvertLowerCaseTest() {
        Reader r = new StringReader(simpleText);
        DocumentIterator d = new DocumentIterator(r, 3);
        String[] answer = {
            "thisisreally",
            "isreallysimple"
        };
        int i = 0;
        
        while (d.hasNext()) {
            assertEquals(d.next(), answer[i++]);
        }
        assertEquals(i, answer.length);
    }

    @Test
    public void singleWordCaseTest() {
        Reader r = new StringReader(simpleText);
        DocumentIterator d = new DocumentIterator(r, 1);   // r has 4 words!
        assertEquals("this", d.next());
        assertEquals("is", d.next());
        assertEquals("really", d.next());
        assertEquals("simple", d.next());
        assertFalse(d.hasNext());
    }

    @Test
    public void nGreaterThanNumOfWordsCaseTest() {
        Reader r = new StringReader(simpleText);
        DocumentIterator d = new DocumentIterator(r, 10);   // r has 4 words!
        assertEquals("", d.next());
    }

    @Test
    public void nEqualToZeroCaseTest() {
        Reader r = new StringReader(simpleText);
        DocumentIterator d = new DocumentIterator(r, 0);
        assertEquals("", d.next());
    }
}