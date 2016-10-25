/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

/**
 *
 * @author Ghostplant
 */
public class Library {

    private int id, databaseTypeAmount;
    private float fgnStoreAmount, chsStoreAmout, fgnPeriodicalAmount, chsPeriodicalAmount, fgnEBookAmount, chsEBookAmount, fgnEPeriodicalAmount, chsEPeriodicalAmount;

    public float getChsEBookAmount() {
        return chsEBookAmount;
    }

    public void setChsEBookAmount(float chsEBookAmount) {
        this.chsEBookAmount = chsEBookAmount;
    }

    public float getChsEPeriodicalAmount() {
        return chsEPeriodicalAmount;
    }

    public void setChsEPeriodicalAmount(float chsEPeriodicalAmount) {
        this.chsEPeriodicalAmount = chsEPeriodicalAmount;
    }

    public float getChsPeriodicalAmount() {
        return chsPeriodicalAmount;
    }

    public void setChsPeriodicalAmount(float chsPeriodicalAmount) {
        this.chsPeriodicalAmount = chsPeriodicalAmount;
    }

    public float getChsStoreAmout() {
        return chsStoreAmout;
    }

    public void setChsStoreAmout(float chsStoreAmout) {
        this.chsStoreAmout = chsStoreAmout;
    }

    public int getDatabaseTypeAmount() {
        return databaseTypeAmount;
    }

    public void setDatabaseTypeAmount(int databaseTypeAmount) {
        this.databaseTypeAmount = databaseTypeAmount;
    }

    public float getFgnEBookAmount() {
        return fgnEBookAmount;
    }

    public void setFgnEBookAmount(float fgnEBookAmount) {
        this.fgnEBookAmount = fgnEBookAmount;
    }

    public float getFgnEPeriodicalAmount() {
        return fgnEPeriodicalAmount;
    }

    public void setFgnEPeriodicalAmount(float fgnEPeriodicalAmount) {
        this.fgnEPeriodicalAmount = fgnEPeriodicalAmount;
    }

    public float getFgnPeriodicalAmount() {
        return fgnPeriodicalAmount;
    }

    public void setFgnPeriodicalAmount(float fgnPeriodicalAmount) {
        this.fgnPeriodicalAmount = fgnPeriodicalAmount;
    }

    public float getFgnStoreAmount() {
        return fgnStoreAmount;
    }

    public void setFgnStoreAmount(float fgnStoreAmount) {
        this.fgnStoreAmount = fgnStoreAmount;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}
