/*
 * CS 61C Fall 2013 Project 1
 *
 * DoublePair.java is a class which stores two doubles and 
 * implements the Writable interface. It can be used as a 
 * custom value for Hadoop. To use this as a key, you can
 * choose to implement the WritableComparable interface,
 * although that is not necessary for credit.
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

import org.apache.hadoop.io.WritableComparable;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class DoublePair implements WritableComparable<DoublePair> {
    // Declare any variables here
    private double d1;
    private double d2;
    /**
     * Constructs a DoublePair with both doubles set to zero.
     */

    public DoublePair() {
        // YOUR CODE HERE
        d1=0;
        d2=0;
    }

    /**
     * Constructs a DoublePair containing double1 and double2.
     */ 
    public DoublePair(double double1, double double2) {
        // YOUR CODE HERE
        this.d1=double1;
        this.d2=double2;
        
    }

    /**
     * Returns the value of the first double.
     */
    public double getDouble1() {
        // YOUR CODE HERE
        return this.d1;
    }

    /**
     * Returns the value of the second double.
     */
    public double getDouble2() {
        // YOUR CODE HERE
        return this.d2;
    }

    /**
     * Sets the first double to val.
     */
    public void setDouble1(double val) {
        // YOUR CODE HERE
        this.d1=val;
    }

    /**
     * Sets the second double to val.
     */
    public void setDouble2(double val) {
        // YOUR CODE HERE
        this.d2=val;
    }

    /**
     * write() is required for implementing Writable.
     */
    public void write(DataOutput out) throws IOException {
        // YOUR CODE HERE
        out.writeDouble(d1);
        out.writeDouble(d2);
    }

    /**
     * readFields() is required for implementing Writable.
     */
    public void readFields(DataInput in) throws IOException {
        // YOUR CODE HERE
        this.setDouble1(in.readDouble());
        this.setDouble2(in.readDouble());
    }

    public int compareTo(DoublePair o) {
        // YOUR CODE HERE
        if (this.d1 < o.d1){
            return -1;
        }
        if (this.d1 > o.d1){
            return 1;
        }
        if (this.d2 < o.d2){
            return -1;
        }
        if (this.d2 > o.d2){
            return 1;
        }
        return 0;
    }    

    public int hashCode() {
        return this.toString().hashCode();
    }

    public String toString(){
         return "" + d1 + " " + d2;
    }    


}
