import { BrowserRouter, Routes, Route } from 'react-router-dom'
import Navbar from './components/Navbar'
import Footer from './components/Footer'
import Home from './pages/Home'
import Download from './pages/Download'
import About from './pages/About'
import Donate from './pages/Donate'

export default function App() {
    return (
        <BrowserRouter basename="/Maryn_Launcher">
            <div className="flex flex-col min-h-screen bg-zinc-950">
                <Navbar />
                <Routes>
                    <Route path="/" element={<Home />} />
                    <Route path="/download" element={<Download />} />
                    <Route path="/about" element={<About />} />
                    <Route path="/donate" element={<Donate />} />
                </Routes>
                <Footer />
            </div>
        </BrowserRouter>
    )
}