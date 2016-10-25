/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

import java.util.HashSet;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class Essay {

    private int id, meetingId, treatiseTotalKWord, treatiseSelfKWord,totalAuthor;

    public int getTotalAuthor() {
        return totalAuthor;
    }

    public void setTotalAuthor(int totalAuthor) {
        this.totalAuthor = totalAuthor;
    }
    private boolean foreignPeriodical;
    private String title, style, type, firstAuthorId, memo, periodicalName, periodicalHolder, periodicalIssnNo,
            periodicalCnNo, periodicalPublish, meetingIsbnNo, treatisePublisher, treatiseBookNo;
    private Set personsOfEssays=new HashSet(),funds=new HashSet();

    public Set getPersonsOfEssays() {
        return personsOfEssays;
    }

    public void setPersonsOfEssays(Set personsOfEssays) {
        this.personsOfEssays = personsOfEssays;
    }

    public Set getFunds() {
        return funds;
    }

    public void setFunds(Set funds) {
        this.funds = funds;
    }

    public String getFirstAuthorId() {
        return firstAuthorId;
    }

    public void setFirstAuthorId(String firstAuthorId) {
        this.firstAuthorId = firstAuthorId;
    }

    public boolean isForeignPeriodical() {
        return foreignPeriodical;
    }

    public void setForeignPeriodical(boolean foreignPeriodical) {
        this.foreignPeriodical = foreignPeriodical;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getMeetingId() {
        return meetingId;
    }

    public void setMeetingId(int meetingId) {
        this.meetingId = meetingId;
    }

    public String getMeetingIsbnNo() {
        return meetingIsbnNo;
    }

    public void setMeetingIsbnNo(String meetingIsbnNo) {
        this.meetingIsbnNo = meetingIsbnNo;
    }

    public String getMemo() {
        return memo;
    }

    public void setMemo(String memo) {
        this.memo = memo;
    }

    public String getPeriodicalCnNo() {
        return periodicalCnNo;
    }

    public void setPeriodicalCnNo(String periodicalCnNo) {
        this.periodicalCnNo = periodicalCnNo;
    }

    public String getPeriodicalHolder() {
        return periodicalHolder;
    }

    public void setPeriodicalHolder(String periodicalHolder) {
        this.periodicalHolder = periodicalHolder;
    }

    public String getPeriodicalIssnNo() {
        return periodicalIssnNo;
    }

    public void setPeriodicalIssnNo(String periodicalIssnNo) {
        this.periodicalIssnNo = periodicalIssnNo;
    }

    public String getPeriodicalName() {
        return periodicalName;
    }

    public void setPeriodicalName(String periodicalName) {
        this.periodicalName = periodicalName;
    }

    public String getPeriodicalPublish() {
        return periodicalPublish;
    }

    public void setPeriodicalPublish(String periodicalPublish) {
        this.periodicalPublish = periodicalPublish;
    }

    public String getStyle() {
        return style;
    }

    public void setStyle(String style) {
        this.style = style;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getTreatiseBookNo() {
        return treatiseBookNo;
    }

    public void setTreatiseBookNo(String treatiseBookNo) {
        this.treatiseBookNo = treatiseBookNo;
    }

    public String getTreatisePublisher() {
        return treatisePublisher;
    }

    public void setTreatisePublisher(String treatisePublisher) {
        this.treatisePublisher = treatisePublisher;
    }

    public int getTreatiseSelfKWord() {
        return treatiseSelfKWord;
    }

    public void setTreatiseSelfKWord(int treatiseSelfKWord) {
        this.treatiseSelfKWord = treatiseSelfKWord;
    }

    public int getTreatiseTotalKWord() {
        return treatiseTotalKWord;
    }

    public void setTreatiseTotalKWord(int treatiseTotalKWord) {
        this.treatiseTotalKWord = treatiseTotalKWord;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
}
