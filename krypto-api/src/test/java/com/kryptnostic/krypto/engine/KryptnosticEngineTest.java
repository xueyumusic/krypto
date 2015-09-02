package com.kryptnostic.krypto.engine;

import java.util.Random;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertArrayEquals;

public class KryptnosticEngineTest {

    @BeforeClass
    public static void setUpBeforeClass() throws Exception {}

    @AfterClass
    public static void tearDownAfterClass() throws Exception {}

    @Before
    public void setUp() throws Exception {}

    @After
    public void tearDown() throws Exception {}

    @Test
    public void testMatrixConversions() throws Exception {
        Random rand = new Random();
        byte[] matrix = new byte[16];
        rand.nextBytes( matrix );
        byte[] output = KryptnosticEngine.testBitMatrixConversion( matrix );
        assertArrayEquals( matrix, output);
    }

    @Test
    public void smokeTest() throws Exception {
        Random rand = new Random();
        byte[] clientHashFunc = new byte[16];
        byte[] encObjectSearchToken = new byte[16];
        rand.nextBytes( clientHashFunc );
        rand.nextBytes( encObjectSearchToken );
        KryptnosticEngine kryptnosticEngine = new KryptnosticEngine( clientHashFunc, encObjectSearchToken );
        assertNotNull( kryptnosticEngine.getHandle() );
        byte[] encObjectSearchKey = new byte[16];
        byte[] objectConversionMatrix = new byte[16];
        kryptnosticEngine.calculateMetadataAddress( encObjectSearchKey, objectConversionMatrix );
    }
}
