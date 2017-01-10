import pandas
import tensorflow
from tensorflow.contrib import learn
from tensorflow.contrib import layers

#tensorflow.logging.set_verbosity(tensorflow.logging.INFO)

data = pandas.read_csv("car.data")
#data = data.dropna()
#y_train, X_train = data['class'], data[['buying','maint', 'doors', 'persons', 'lug_boot', 'safety']].fillna(0)

data_test = pandas.read_csv("car_test.data")
#data = data.dropna()
#y_test, X_test = data['class'], data[['buying','maint', 'doors', 'persons', 'lug_boot', 'safety']].fillna(0)

features = ["buying", "maint", "doors", "persons", "lug_boot", "safety"]
classes={"unacc":0,"acc":1,"good":2,"vgood":3}

def help(c):
    return classes[c]
	
def input_fn(df):   
    col={k: tensorflow.SparseTensor(
        indices=[[i,0] for i in range(df[k].size)],
        values=df[k].values,
        shape=[df[k].size,1]) for k in features}
    label=tensorflow.constant(df["tmp"].values)
    return col,label

def train_input_fn():
	return input_fn(data)	
	
def testing_fn():
	return input_fn(data_test)		

data["tmp"]= data["class"].apply(help)
data_test["tmp"]= data_test["class"].apply(help)

buying = layers.sparse_column_with_keys(column_name="buying",keys=["low","med","high","vhigh"])
maint = layers.sparse_column_with_keys(column_name="maint",keys=["low","med","high","vhigh"])
doors = layers.sparse_column_with_keys(column_name="doors",keys=["2","3","4","5more"])
persons = layers.sparse_column_with_keys(column_name="persons",keys=["2","4","more"])
lug_boot = layers.sparse_column_with_keys(column_name="lug_boot",keys=["small","med","big"])
safety = layers.sparse_column_with_keys(column_name="safety",keys=["low","med","high"])

buying_emb = layers.embedding_column(buying,dimension=4)
maint_emb = layers.embedding_column(maint,dimension=4)
doors_emb = layers.embedding_column(doors,dimension=4)
persons_emb = layers.embedding_column(persons,dimension=3)
lug_boot_emb = layers.embedding_column(lug_boot,dimension=3)
safety_emb = layers.embedding_column(safety,dimension=3)

dnn_classifier = learn.DNNClassifier(feature_columns=[buying_emb, maint_emb,doors_emb,persons_emb,lug_boot_emb,safety_emb], hidden_units=[10], n_classes=4, )
#dnn_classifier.fit(X_train, y_train, steps = 1000)
dnn_classifier.fit(input_fn=train_input_fn,steps=1000)

y_test = dnn_classifier.evaluate(input_fn=testing_fn,steps=1)

print("Precision=")
print(y_test['accuracy'])
