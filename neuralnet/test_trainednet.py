import math
import os

import matplotlib
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.autograd import Variable
from torch.utils.data import DataLoader, Dataset
from torchvision import transforms
from sklearn.metrics import confusion_matrix
import random
import matplotlib.pyplot as plt
from mpl_toolkits import axes_grid1

import pickle
import warnings

#warnings.filterwarnings("error")

# PyTorch
class DiceLoss(nn.Module):
    def __init__(self, weight=None, size_average=True):
        super(DiceLoss, self).__init__()

    def forward(self, inputs, targets, smooth=1):
        # comment out if your model contains a sigmoid or equivalent activation layer
        #inputs = F.sigmoid(inputs)

        # flatten label and prediction tensors
        inputs = inputs.view(-1)
        targets = targets.view(-1)

        intersection = (inputs * targets).sum()
        dice = (2. * intersection + smooth) / (inputs.sum() + targets.sum() + smooth)

        return 1 - dice
# encoding
class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv2d(5, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        self.conv3 = nn.Conv2d(64, 128, kernel_size=3, padding=1)
        self.mp = nn.MaxPool2d(2)
        self.fc = nn.Linear(33280, 2)

    def forwardxd(self, x):
        in_size = x.size(0)
        x1 = F.relu(self.conv1(x))
        x1 = self.mp(x1)  # size=(N, 32, x.H/2, x.W/2)
        x2 = F.relu(self.conv2(x1))
        x2 = self.mp(x2)  # size=(N, 64, x.H/4, x.H/4)
        x3 = F.relu(self.conv3(x2))
        x3 = self.mp(x3)  # size=(N, 128, x.H/8, x.H/8)
        x4 = x3.view(in_size, -1)
        x4 = self.fc(x4)  # size=(N, n_class)
        y = F.log_softmax(x4, dim=0)  # size=(N, n_class)
        return x1, x2, x3, x4, y


# encoding/decoding
class FCN8s(nn.Module):
    # Po-Chih Huang
    def __init__(self, pretrained_net, n_class):
        super(FCN8s, self).__init__()
        self.n_class = n_class
        self.pretrained_net = pretrained_net
        self.relu = nn.ReLU(inplace=True)
        self.deconv1 = nn.ConvTranspose2d(128, 64, kernel_size=3, stride=2, padding=1, dilation=1, output_padding=1)
        self.bn1 = nn.BatchNorm2d(64)
        self.deconv2 = nn.ConvTranspose2d(64, 32, kernel_size=3, stride=2, padding=1, dilation=1, output_padding=1)
        self.bn2 = nn.BatchNorm2d(32)
        self.deconv3 = nn.ConvTranspose2d(32, 16, kernel_size=4, stride=2, padding=1, dilation=1, output_padding=1)
        self.bn3 = nn.BatchNorm2d(16)
        self.classifier = nn.Conv2d(16, n_class, kernel_size=1)

    def forward(self, x):
        output = self.pretrained_net.forwardxd(x)
        x3 = output[2]
        x2 = output[1]
        x1 = output[0]
        score = self.relu(self.deconv1(x3))
        score = self.bn1(score + x2)
        score = self.relu(self.deconv2(score))
        score = self.bn2(score + x1)
        score = self.bn3(self.relu(self.deconv3(score)))
        score = self.classifier(score)
        return score


def train(epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(train_loader):
        data, target = Variable(data).float().to(DEVICE), Variable(target).float().to(DEVICE)
        data = torch.cuda.FloatTensor(data)
        optimizer.zero_grad()
        output = model(data)
        output2 = torch.sigmoid(output)
        loss = loss_fn(output2, target)
        loss.backward()
        optimizer.step()
        if batch_idx % 10 == 0:
            print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx * len(data), len(train_loader.dataset),
                       100. * batch_idx / len(train_loader), loss.item()))


def iou(p, t):
    p = p.cpu()
    t = t.cpu()
    current = confusion_matrix(t.numpy().flatten(), p.numpy().flatten(), labels=[0, 1])
    intersection = np.diag(current)
    ground_truth_set = current.sum(axis=1)  # rows
    predicted_set = current.sum(axis=0)  # columns
    union = ground_truth_set + predicted_set - intersection
    IoU = intersection / union.astype(np.float32)
    return np.mean(IoU), current, IoU[0], IoU[1]


def pixel_acc(p, t):
    p = p.cpu()
    t = t.cpu()
    correct_pixels = (p == t).sum().to(dtype=torch.float)
    total_pixels = (t == t).sum().to(dtype=torch.float)
    return correct_pixels / total_pixels


def calc_acc(p, t):
    p = p.cpu()
    t = t.cpu()
    correct_pixels = (p == t).sum().to(dtype=torch.float)
    total_pixels = (t == t).sum().to(dtype=torch.float)
    return correct_pixels / total_pixels


def calc_iou(p, t):
    p = p.cpu()
    t = t.cpu()
    current = confusion_matrix(t.numpy().flatten(), p.numpy().flatten(), labels=[0, 1])
    intersection = np.diag(current)
    ground_truth_set = current.sum(axis=1)  # rows
    predicted_set = current.sum(axis=0)  # columns
    union = ground_truth_set + predicted_set - intersection
    IoU = intersection / union.astype(np.float32)
    tp = IoU[1]
    tn = IoU[0]
    if math.isnan(tp):
        tp = 0
    if math.isnan(tn):
        tn = 0
    iou = (tp + tn) / 2
    return tp, tn, iou


def calc_wiou(p, t):
    N = len(t)
    p = p.cpu()
    t = t.cpu()
    w_TP = np.zeros(N)
    w_TN = np.zeros(N)
    IoU = np.zeros(N)
    IoU_TP = np.zeros(N)
    IoU_TN = np.zeros(N)
    for j in range(N):
        pred, tar = p[j], t[j]
        w_TP[j] = sum(sum(tar))
        w_TN[j] = sum(sum(1 - tar))
        IoU_TP[j], IoU_TN[j], IoU[j] = calc_iou(p=pred, t=tar)
    wIoU_TP = sum(w_TP * IoU_TP) / sum(w_TP)
    if math.isnan(wIoU_TP):
        wIoU_TP = 0
    wIoU_TN = sum(w_TN * IoU_TN) / sum(w_TN)
    if math.isnan(wIoU_TN):
        wIoU_TN = 0
    return wIoU_TP, wIoU_TN, IoU_TP, IoU_TN, IoU


def listMean(my_list, mean_type):
    if mean_type == 1:
        avg1 = [float(sum(col))/len(col) for col in zip(*my_list)]
        return sum(avg1)/len(avg1)
    if mean_type == 2:
        return sum(my_list)/len(my_list)


def round_metrics(a, b, c, d, e, f, r):
    return round(a, r)*100, round(b, r)*100, round(c, r)*100, round(d, r)*100, round(e, r)*100, round(f, r)*100


def test(doSave, threshold):
    model.eval()
    n_batches = 0
    acc = []
    iou_mn = []
    iou_tp = []
    iou_tn = []
    wiou_tp = []
    wiou_tn = []
    all_targets = []
    all_out = []
    all_output = []
    all_pred = []
    for data, target in test_loader:
        n_batches += 1
        data, target = \
            Variable(data).float().to(DEVICE), Variable(target).float().to(DEVICE)
        out = model(data)
        output = torch.sigmoid(out)
        pred = (output[:, 1, :, :] > threshold).float() * 1
        acc.append(calc_acc(p=pred.long(), t=target[:, 1, :, :].long()))
        iou_out = calc_wiou(p=pred, t=target[:, 1, :, :])
        wiou_tp.append(iou_out[0])
        wiou_tn.append(iou_out[1])
        iou_tp.append(iou_out[2])
        iou_tn.append(iou_out[3])
        iou_mn.append(iou_out[4])
        if doSave:
            all_targets.append(target[:, 1, :, :])
            all_out.append(out[:, 1, :, :])
            all_output.append(output[:, 1, :, :])
            all_pred.append(pred)
    aa = np.array(acc).mean()
    bb = listMean(iou_mn, 1)
    cc = listMean(iou_tp, 1)
    dd = listMean(wiou_tp, 2)
    ee = listMean(iou_tn, 1)
    ff = listMean(wiou_tn, 2)
    print('\nTest Epoch: {}\t Accuracy: {}%, IoU: {}%, TP IoU: {}({})%, TN IoU = {}({})%\n'.format(
        epoch, round(100 * aa, 2), round(100 * bb, 2), round(100 * cc, 2), round(100 * dd, 2),
        round(100 * ee, 2), round(100 * ff, 2)))
    return all_targets, all_out, all_output, all_pred


torch.cuda.empty_cache()
DEVICE = "cpu"
# network settings
batch_size = 1
n_class = 2

# set threshold
thres = torch.Tensor([.666]).to(DEVICE)  # try: 0, -.2, -.1, .1, .2, .3, .4
flnm = "666"

dataset_dir = "./landsat_datasets_manual_5bands/"
# test_filename = "./output/landsat_dataset_scene2.p"
# train_filename = "./output/landsat_dataset.p"
# test_f = open(test_filename, "rb")
# train_f = open(train_filename, "rb")
full_dataset = []
for str in os.listdir(dataset_dir):
    f = open(dataset_dir+str,"rb")
    dataset = pickle.load(f)
    f.close()
    full_dataset.extend(dataset)


#val_set = pickle.load(test_f)
#train_dataset = pickle.load(train_f)
#test_f.close()
#train_f.close()
fifteenpercent = int(len(full_dataset)*0.15)
train_set, val_set = torch.utils.data.random_split(full_dataset, [len(full_dataset)-fifteenpercent, fifteenpercent])
# data loader
random.seed(1319)
train_loader = torch.utils.data.DataLoader(dataset=train_set, batch_size=batch_size, shuffle=True)
test_loader = torch.utils.data.DataLoader(dataset=val_set, batch_size=batch_size, shuffle=False)

cnn_model = CNN().to(DEVICE)
model = FCN8s(pretrained_net=cnn_model, n_class=n_class).to(DEVICE)
model.load_state_dict(torch.load('./output/trained_net.p'))
model.eval()
optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.5)
weights = torch.tensor([1, 19], dtype=torch.float32)
weights = weights / weights.sum()
weights = 1.0 / weights
weights = weights / weights.sum()
#loss_fn = nn.CrossEntropyLoss(weight=weights).to(DEVICE)
#loss_fn = nn.BCEWithLogitsLoss()
loss_fn = DiceLoss()

# run for NT Data Set
epoch = 1
n_epochs = 1
results = test(doSave=((epoch) == n_epochs), threshold=thres)

test_dat = []
for dat, _ in test_loader:
    test_dat.append(dat)

results_filename = './output/results.p'
results_f = open(results_filename, 'wb')
pickle.dump(results, results_f)
results_f.close()

matplotlib.use('Agg')

# plotting
for i in range(len(results[0])):
    X = test_dat[i].cpu()
    Y = results[1][i].cpu()
    Yest = results[2][i].cpu()
    Yhat = results[3][i].cpu()
    Z = results[0][i].cpu()
    for j in range(len(X)):
        blue = X[j, 0, :, :].detach().numpy()
        green = X[j, 1, :, :].detach().numpy()
        red = X[j, 2, :, :].detach().numpy()
        ir = X[j, 3, :, :].detach().numpy()
        swir = X[j, 4, :, :].detach().numpy()
        # clouds = X[j, 5, :, :].detach().numpy()
        # temp = X[j, 6, :, :].detach().numpy()
        img = np.array([red, green, blue])
        img = np.moveaxis(img, 0, -1)
        for n in range(3):
            mx = np.max(img[:, :, n])
            mn = np.min(img[:, :, n])
            img[:, :, n] = (img[:, :, n] - mn) / (mx - mn)
        y = Y[j, :, :].detach().numpy()
        z = Z[j, :, :].detach().numpy()
        yhat = Yhat[j, :, :].detach().numpy()
        yest = Yest[j, :, :].detach().numpy()
        plt.subplot(2, 3, 1)
        plt.imshow(img)
        plt.axis("off")
        plt.title("RGB composite")
        plt.subplot(2, 3, 2)
        plt.imshow(red)
        plt.axis("off")
        plt.title("Red")
        plt.subplot(2, 3, 3)
        plt.imshow(ir)
        plt.axis("off")
        plt.title("NIR")
        plt.subplot(2, 3, 4)
        plt.imshow(swir)
        plt.axis("off")
        plt.title("TIR")
        # plt.subplot(2, 4, 5)
        # plt.imshow(clouds)
        # plt.subplot(2, 4, 6)
        # plt.imshow(temp)
        plt.subplot(2, 3, 5)
        plt.imshow(z)
        plt.axis("off")
        plt.title("Ground truth")
        plt.subplot(2, 3, 6)
        plt.imshow(yhat)
        plt.axis("off")
        plt.title("Prediction")
        plt.savefig('./allplots/fig{}{}.png'.format(i, j))
        plt.close('all')
