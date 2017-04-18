import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        self.negatives = []
        file1 = open(positives,"r")
        for line in file1:
            if not line.startswith(";"):
                self.positives.append(line.rstrip("\n"))
        file1.close()
        
        file2 = open(negatives,"r")
        for line in file2:
            if not line.startswith(";"):
                self.negatives.append(line.rstrip("\n"))
        file2.close()
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        counter = 0
        #Splits a single tweet str into a list of words   
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        for i in tokens:
            if i.lower() in self.positives:
                counter += 1
            
            if i.lower() in self.negatives:
                counter += -1
        return counter
            
